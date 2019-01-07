#ifndef MIO_NODE_BASE_H
#define MIO_NODE_BASE_H
#include <napi.h>
#include <iostream>
#include "mio/mmap.hpp"

#ifdef _WIN32
#include <io.h>
bool file_exists(const std::string &file)
{
    return _access_s(file.c_str(), 0) == 0;
}
#else
#include <unistd.h>
bool file_exists(const std::string &file)
{
    return access(file.c_str(), 0) == 0;
}
#endif

static inline int64_t get_int64(const Napi::Value &v)
{
    return v.As<Napi::Number>().Int64Value();
}

template <mio::access_mode AccessMode>
class MappedFile : public Napi::ObjectWrap<MappedFile<AccessMode>>
{
  public:
    static Napi::FunctionReference constructor;

    MappedFile(const Napi::CallbackInfo &info) : Napi::ObjectWrap<MappedFile<AccessMode>>(info)
    {
        Napi::Env env = info.Env();
        if (info.Length() < 1)
        {
            throw Napi::TypeError::New(env, "param <file> expected");
        }
        size_t offset = info.Length() > 1 ? get_int64(info[1]) : 0;
        size_t size_to_map = info.Length() > 2 ? get_int64(info[2]) : (size_t)mio::map_entire_file;
        std::error_code err;
        if (info[0].IsNumber())
        {
            auto fd = (mio::file_handle_type)(get_int64(info[0]));
            this->mmap.map(fd, offset, size_to_map, err);
        }
        else if (info[0].IsString())
        {
            auto fd = info[0].As<Napi::String>().Utf8Value();
            if (!file_exists(fd))
            {
                throw Napi::TypeError::New(env, "file not exists");
            }
            this->mmap.map(fd, offset, size_to_map, err);
        }
        else
        {
            throw Napi::TypeError::New(env, "file should be string or fd");
        }
        if (err)
        {
            throw Napi::TypeError::New(env, err.message());
        }
    }

    Napi::Value buffer(const Napi::CallbackInfo &info)
    {
        const size_t offset = info.Length() > 0 ? get_int64(info[0]) : 0;
        const size_t len = info.Length() > 1 ? get_int64(info[1]) : this->mmap.size() - offset;
        return Napi::Buffer<uint8_t>::New(info.Env(), (uint8_t *)this->mmap.cbegin() + offset, len);
    }

    Napi::Value writable(const Napi::CallbackInfo &info)
    {
        return Napi::Boolean::New(info.Env(), AccessMode == mio::access_mode::write);
    }

    void sync(const Napi::CallbackInfo &info);

    void unmap(const Napi::CallbackInfo &info)
    {
        if (this->mmap.is_mapped())
        {
            this->mmap.unmap();
        }
    }

    Napi::Value isMapped(const Napi::CallbackInfo &info)
    {
        return Napi::Number::New(info.Env(), this->mmap.is_mapped());
    }
    Napi::Value size(const Napi::CallbackInfo &info)
    {
        return Napi::Number::New(info.Env(), (double)this->mmap.size());
    }
    Napi::Value offset(const Napi::CallbackInfo &info)
    {
        return Napi::Number::New(info.Env(), (double)this->mmap.offset());
    }
    ~MappedFile()
    {
        if (this->mmap.is_mapped())
        {
            this->mmap.unmap();
        }
    }

  protected:
    mio::basic_mmap<AccessMode, unsigned char> mmap;
    void _sync(const Napi::Env &env);
};
template <>
void MappedFile<mio::access_mode::write>::sync(const Napi::CallbackInfo &info)
{
    std::error_code err;
    this->mmap.sync(err);
    if (err)
    {
        throw Napi::TypeError::New(info.Env(), err.message());
    }
}
template <>
void MappedFile<mio::access_mode::read>::sync(const Napi::CallbackInfo &info)
{
    // noop
}

using RW = MappedFile<mio::access_mode::write>;
using RO = MappedFile<mio::access_mode::read>;

template <>
Napi::FunctionReference RW::constructor = Napi::FunctionReference();

template <>
Napi::FunctionReference RO::constructor = Napi::FunctionReference();

inline void
MioInit(Napi::Env env, Napi::Object exports)
{
    Napi::Function rw_constructor = RW::DefineClass(env, "MappedFile", {
                                                                           RW::InstanceAccessor("writable", &RW::writable, nullptr),
                                                                           RW::InstanceAccessor("length", &RW::size, nullptr),
                                                                           RW::InstanceAccessor("offset", &RW::offset, nullptr),
                                                                           RW::InstanceMethod("buffer", &RW::buffer),
                                                                           RW::InstanceMethod("size", &RW::size),
                                                                           RW::InstanceMethod("isMapped", &RW::isMapped),
                                                                           RW::InstanceMethod("unmap", &RW::unmap),
                                                                           RW::InstanceMethod("sync", &RW::sync),
                                                                       });

    RW::constructor = Napi::Persistent(rw_constructor);
    RW::constructor.SuppressDestruct();
    exports.Set("MappedFile", rw_constructor);

    Napi::Function ro_constructor = RO::DefineClass(env, "ReadOnlyMappedFile", {
                                                                                   RO::InstanceAccessor("writable", &RO::writable, nullptr),
                                                                                   RO::InstanceAccessor("length", &RO::size, nullptr),
                                                                                   RO::InstanceAccessor("offset", &RO::offset, nullptr),
                                                                                   RO::InstanceMethod("buffer", &RO::buffer),
                                                                                   RO::InstanceMethod("size", &RO::size),
                                                                                   RO::InstanceMethod("isMapped", &RO::isMapped),
                                                                                   RO::InstanceMethod("unmap", &RO::unmap),
                                                                                   RO::InstanceMethod("sync", &RO::sync),
                                                                               });
    RW::constructor = Napi::Persistent(ro_constructor);
    RW::constructor.SuppressDestruct();
    exports.Set("ReadOnlyMappedFile", ro_constructor);
}

#endif