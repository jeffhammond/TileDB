/*
** Created by Seth Shelnutt on 6/18/18.
** Licensed under the GNU Lesser General Public License v3 or later
*/

#ifndef TILEDB_CLIENT_H
#define TILEDB_CLIENT_H

#include <tiledb/sm/misc/status.h>

/**
 * Get a json encoded array schema from rest server
 *
 * @param rest_server url
 * @param uri of array being loaded
 * @param jsonReturned string where json response is stored
 * @return Status Ok() on success Error() on failures
 */
tiledb::sm::Status get_array_schema_json_from_rest(
    std::string rest_server, std::string uri, char** jsonReturned);

/**
 * Post a json array schema to rest server
 *
 * @param rest_server url
 * @param uri of array being created
 * @param jsonReturned string of json serialized array
 * @return Status Ok() on success Error() on failures
 */
tiledb::sm::Status post_array_schema_json_to_rest(
    std::string rest_server, std::string uri, char* json);

#endif  // TILEDB_CLIENT_H
