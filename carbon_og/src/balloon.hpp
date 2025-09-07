#pragma once
/**
 *
 * BALLOON - C++ lightweight zlib implementation
 * 
 * Current Version: 1.1.2 / dev1.2
 *
 * Version 0.0 written May 2024 [Old]
 * 		poorly made
 * Version 1.0 written September - December 2024 [Old]
 *      made better
 * Version 1.1 written June 2025 [New]
 * 	    slight optimizations with bugs fixes
 * Version 1.1.2 written June 28th 2025 [New]
 * 		fixed small part of inflate where the lz77 algorithm can reference data from prev blocks
 * Version 1.2  written June 18th 2025 [In Developement]
 * 		optimizations
 *
 * Program written by muffinshades
 *
 * Copyright (c) 2024-Present muffinshades
 *
 * You can do what ever you want with the software but you must
 * credit the author (muffinshades) with the original creation
 * of the software.
 * 
 * All modifications of this piece of software must be noted in
 * this header file
 *
 * Balloon Notes:
 *
 * This library is a implementation of the zlib or Inflate / Deflate
 * compression algorithm.
 *
 * Right now compression speeds are around 8-10mb/s and decompression
 * speeds are much faster. This isn't the worlds fastest implementation,
 * but its decently fast and lightweight.
 *
 * This program should be able to function with any other inflate / deflate
 * implementation apart from the whole compression level calculations being
 * different since I didn't entirley implement lazy and good matches into
 * the lz77 functions. I also didn't add a whole fast version for everything
 * since this is a relativley light weight library. One day I do plan on adding
 * these functions and making a even better implementation of zlib.
 *
 */

#include <iostream>
#include <cstring>
#include "msutil.hpp"
#include "bitstream.hpp"

#ifdef CARBON_DLL
#ifdef CARBON_EXPORTS
#define CARBON_EXP __declspec(dllexport)
#else
#define CARBON_EXP __declspec(dllimport)
#endif
#else
#define CARBON_EXP
#endif

#ifdef CARBON_DLL
#ifdef __cplusplus
extern "C" {
#endif
#endif

struct balloon_result {
	byte* data;
	size_t sz;
	u32 checksum;
	byte compressionMethod;
};

struct huffman_node {
	huffman_node* left = nullptr, * right = nullptr; //left and right pointers
	u32 val = 0;
	size_t count = 0, depth = 0, cmpCount = 0;
	bool leaf = false;
	i32* symCodes = nullptr;
	size_t alphaSz = 0;
};

struct huffman_tree {
	huffman_node* root;
	u32* bitLens;
	size_t alphaSz;
};

class Huffman {
public:
	static void TreeFree(huffman_node *root);
	static void TreeFree(huffman_tree *tree);
	static void InsertNodeIntoTree(const u64 code, const size_t codeLen, const u64 val, huffman_node* root);
	static void InsertNodeIntoTree(const u64 code, const size_t codeLen, const u64 val, huffman_tree* tree);
	static huffman_node* GenTreeFromCounts(size_t* counts, size_t alphaSz, i32 maxLen = -1);
	static u32 *GetTreeBitLens(huffman_node* tree, size_t alphabetSize, i32 currentLen = 0, u32* cbl = nullptr);
	static size_t* GetBitLenCounts(u32* bitLens, const size_t blLen, const size_t MAX_BITS);
	static huffman_node* GenCanonicalTreeFromCounts(size_t* counts, size_t alphaSz, i32 maxLen = -1);
	static huffman_node* CovnertTreeToCanonical(huffman_node* tree, size_t alphabetSize, bool deleteOld = false);
	static huffman_node* BitLengthsToTree(u32* bitLens, size_t blLen, size_t aLen);
	static huffman_tree GenCanonicalTreeAndInfFromCounts(size_t* counts, size_t alphaSz, i32 maxLen = -1);
	static u32 EncodeSymbol(u32 sym, huffman_node* tree);
	static u32 DecodeSymbol(BitStream* stream, huffman_node* tree);
};

#define BALLOON_MAX_THREADS_AUTO -1

class Balloon {
public:
	CARBON_EXP static balloon_result Deflate(byte* data, size_t sz, u32 compressionLevel = 2, const size_t winBits = 0xf);
	CARBON_EXP static balloon_result Inflate(byte* data, size_t sz);
	static bool DeflateFileToFile(std::string in_src, std::string out_src, u32 compressionLevel = 2, const size_t winBits = 0xf);
	static bool InflateFileToFile(std::string in_src, std::string out_src);
	static balloon_result MultiThreadDeflate(byte* data, size_t sz, i32 maxThreads = BALLOON_MAX_THREADS_AUTO, u32 compressionLevel = 2, const size_t winBits = 0xf);
	static balloon_result MultiThreadDeflate(byte* data, size_t sz, i32 maxThreads = BALLOON_MAX_THREADS_AUTO);
	static bool MultiThreadDeflateFileToFile(std::string in_src, std::string out_src, i32 maxThreads = BALLOON_MAX_THREADS_AUTO, u32 compressionLevel = 2, const size_t winBits = 0xf);
	static bool MultiThreadInflateFileToFile(std::string in_src, std::string out_src, i32 maxThreads = BALLOON_MAX_THREADS_AUTO);
	CARBON_EXP static void Free(balloon_result* res);
};

#ifdef MSFL_DLL
#ifdef __cplusplus
}
#endif
#endif