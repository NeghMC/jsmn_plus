/*
 * MIT License
 *
 * Copyright (c) 2025 Grzegorz P. Świstak
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

#include "jsmn_base.h"
#include "string.h"

int jsmn_isTokenString(const unsigned char *json, const jsmntok_t *tok, const char *name)
{
	size_t tokenLength = tok->end - tok->start;
    if (tok->type == JSMN_STRING &&
        (int)strlen(name) == tokenLength &&
        strncmp((const char *)(json + tok->start), name, tokenLength) == 0)
    {
        return 0; // match
    }
    return -1; // no match
}

jsmntok_t * jsmn_getValueFromObject(const unsigned char *json, const jsmntok_t *token, const char *name)
{
	if(token->type != JSMN_OBJECT || token->size == 0)
	{
		return NULL;
	}

	const jsmntok_t *current = token + 1;
	int i = 0;
	for(;;)
	{
		// validate name
		if(jsmn_isTokenString(json, current, name) == 0)
		{
			if(current->size == 1)
			{
				return (jsmntok_t *)current + 1;
			}
			else
			{
				return NULL;
			}
		}
		i++;
		if(i >= token->size)
		{
			break;
		}

		// jump to next token
		for(int j = 1; j > 0; --j)
		{
			j += current->size;
			current += 1;
		}
	}
	return NULL;
}
