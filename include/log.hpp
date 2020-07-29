/*
 * gravity
 * Copyright (C) 2017  Javier Lancha Vázquez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

// Log definitions
#ifndef LOG_LEVEL
#   define LOG_LEVEL 1
#endif

#define LOGI
#define LOGE
#define LOGD
#define LOGV
#define LOGVV

#if LOG_LEVEL >= 4
#   define LOGVV printf("[VV] "); \
    printf
#endif

#if LOG_LEVEL >= 3
#   define LOGV printf("[V] "); \
    printf
#endif

#if LOG_LEVEL >= 2
#   define LOGD printf("[D] "); \
    printf
#endif

#if LOG_LEVEL >= 1
#   define LOGI printf("[I] "); \
    printf
#endif

#if LOG_LEVEL >= 0
#   define LOGE printf("[E] "); \
    printf
#endif
