## mmap-mio
A cross-platform memory-mapped library for nodejs(& TypeScript).  
powered by [mio (C++11 cross-platform memory mapping library)](https://github.com/mandreyel/mio)
and [node-addon-api](https://github.com/nodejs/node-addon-api)  

* TypeScript support.
* only support node 8.0+.
* gc friendly.


### Usage
```javascript
const { mmap ,mmap_ro } = require('mmap-mio') 
const m = mmap('./file.hex')
const ro = mmap_ro('./file.hex') // for read-only mode 
const buffer = m.buffer() // get file Buffer
// buffer.toString()
// buffer[0] = 65
// const byte = buffer[123]
m.sync() // force flush changes to disk.
m.unmap() // unmap the file. Note: do not access the buffer after unmapped;

```

### api
```TypeScript
/**
 * Creates an instance of MappedFile.
 *
 * @export
 * @param {(string | number)} path_or_fd
 * @param {number} [offset]
 * @param {number} [size_to_map]
 * @returns {MappedFile}
 */
function mmap(path_or_fd: string | number, offset?: number, size_to_map ?: number): MappedFile

/**
 * Creates an instance of ReadOnlyMappedFile.
 *
 * @export
 * @param {(string | number)} path_or_fd
 * @param {number} [offset]
 * @param {number} [size_to_map]
 * @returns {ReadOnlyMappedFile}
 */
function mmap_ro(path_or_fd: string | number, offset ?: number, size_to_map ?: number): ReadOnlyMappedFile

// same as ReadOnlyMappedFile
class MappedFile {

  constructor(path_or_fd: string | number, offset ?: number, size_to_map ?: number);

  readonly writable: boolean

  readonly length: number // mapped length

  readonly offset: number

  buffer(offset?:number,length?:number): Buffer // get the buffer of mapped data

  size(): number  // mapped length same with .length

  isMapped(): boolean  

  unmap(): void  // unmap the file

  sync(): void  // flush changes to disk

}

```

