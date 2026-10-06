/*
 * MIT License
 *
 * Copyright (c) 2026 Grzegorz P. Świstak
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>
#include <stdint.h>
#define JSMN_STRICT
#include "jsmn_base.h"

int jsmn_isString(const char *json, const jsmntok_t *token, const char *name)
{
	uint16_t tokenLength = (uint16_t)token->end - (uint16_t)token->start;
    if (token->type == JSMN_STRING && strncmp((const char *)(json + token->start), name, tokenLength) == 0)
    {
        return 1; // match
    }
    return 0; // no match
}

uint16_t jsmn_getNextKey(const jsmntok_t * startingToken, uint16_t currentKeyOffset)
{
    uint16_t nextKey = currentKeyOffset + 1;

    while (currentKeyOffset < nextKey)
    {
        nextKey += (uint16_t)startingToken[currentKeyOffset].size;
        currentKeyOffset++;
    }

    return currentKeyOffset;
}

jsmntok_t * jsmn_getValueFromObject(const char *json, jsmntok_t *object, const char *key)
{
	if(object->type != JSMN_OBJECT || object->size == 0)
	{
		return NULL;
	}

	uint16_t currentKey = 1;
	for(uint16_t i = 0; i < object->size; ++i)
	{
		currentKey = jsmn_getNextKey(object + currentKey, 0);
		if(jsmn_isString(json, object + currentKey, key))
		{
			return object + currentKey;
		}
	}
	return NULL;
}
