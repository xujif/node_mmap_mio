{
    "targets": [
        {
            "target_name": "mmap_mio",
            'cflags!': ['-fno-exceptions'],
            'cflags!_cc': ['-fno-exceptions'],
            'cflags': ['-fexceptions', '-Wall', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE', '-O2'],
            'cflags_cc': ['-fexceptions', '-Wall', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE', '-O2'],
            "sources": ["src/addon.cpp"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "mio/include"
            ],
            'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                'CLANG_CXX_LIBRARY': 'libc++',
                'MACOSX_DEPLOYMENT_TARGET': '10.7',
            },
            'msvs_settings': {
                'VCCLCompilerTool': {
                    'ExceptionHandling': 1,
                    'AdditionalOptions': [
                        '/EHsc'
                    ]
                },
            },
            'defines': ['NAPI_CPP_EXCEPTIONS'],
        }
    ]
}
