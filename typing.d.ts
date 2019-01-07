
export declare class MappedFile {

  /**
   * Creates an instance of MappedFile.
   * @param {(string | number)} path_or_fd
   * @param {number} [offset]
   * @param {number} [size_to_map]
   * @memberof MappedFile
   */
  constructor(path_or_fd: string | number, offset?: number, size_to_map?: number);

  readonly writable: boolean

  /**
   * mapped length
   *
   * @type {number}
   * @memberof MappedFile
   */
  readonly length: number

  readonly offset: number

  /**
   * get the buffer of mapped data
   *
   * @param {number} [offset]
   * @param {number} [length]
   * @returns {Buffer}
   * @memberof MappedFile
   */
  buffer(offset?: number, length?: number): Buffer

  /**
   * mapped length
   *
   * @returns {number}
   * @memberof MappedFile
   */
  size(): number

  isMapped(): boolean

  unmap(): void

  sync(): void

}

export declare class ReadOnlyMappedFile extends MappedFile { }

/**
 * Creates an instance of MappedFile.
 *
 * @export
 * @param {(string | number)} path_or_fd
 * @param {number} [offset]
 * @param {number} [size_to_map]
 * @returns {MappedFile}
 */
export declare function mmap(path_or_fd: string | number, offset?: number, size_to_map?: number): MappedFile

/**
 * Creates an instance of ReadOnlyMappedFile.
 *
 * @export
 * @param {(string | number)} path_or_fd
 * @param {number} [offset]
 * @param {number} [size_to_map]
 * @returns {ReadOnlyMappedFile}
 */
export declare function mmap_ro(path_or_fd: string | number, offset?: number, size_to_map?: number): ReadOnlyMappedFile

export default mmap
