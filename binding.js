const addon = require('bindings')('mmap_mio');

Object.defineProperty(exports, "__esModule", { value: true });

exports.MappedFile = addon.MappedFile
exports.ReadOnlyMappedFile = addon.ReadOnlyMappedFile

exports.mmap = function mmap (file, mode = 'rw', offset = 0, size_to_map = 0) {
  return new addon.MappedFile(file, offset, size_to_map);
}

exports.mmap_ro = function mmap (file, offset = 0, size_to_map = 0) {
  return new addon.ReadOnlyMappedFile(file, offset, size_to_map);
}

exports.default = exports.mmap
