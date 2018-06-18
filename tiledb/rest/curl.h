/*
** Created by Seth Shelnutt on 6/17/18.
** Licensed under the GNU Lesser General Public License v3 or later
*/

#ifndef TILEDB_REST_CURL_HPP
#define TILEDB_REST_CURL_HPP

#include <curl/curl.h>
#include <cstdlib>
#include <string>

struct MemoryStruct {
  char* memory;
  size_t size;
};

/**
 *
 * Helper callback function from libcurl examples
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
size_t WriteMemoryCallback(
    void* contents, size_t size, size_t nmemb, void* userp);

/**
 * Help to make url fetches
 * @param curl pointer to curl instance
 * @param url to post/get
 * @param fetch data for response
 * @return
 */
CURLcode curl_fetch_url(
    CURL* curl, const char* url, struct MemoryStruct* fetch);

/**
 *
 * Simple wrapper for posting json to server
 *
 * @param curl instance
 * @param url to post to
 * @param jsonString json encoded string for posting
 * @param memoryStruct where response is stored
 * @return
 */
CURLcode post_json(
    CURL* curl,
    std::string url,
    std::string jsonString,
    MemoryStruct* memoryStruct);

/**
 * Simple wrapper for getting json from server
 *
 * @param curl instance
 * @param url to get
 * @param memoryStruct response is stored
 * @return
 */
CURLcode get_json(CURL* curl, std::string url, MemoryStruct* memoryStruct);

#endif  // TILEDB_REST_CURL_HPP
