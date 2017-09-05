/*
 * Copyright (c) 1997, 98, 2000  Motoyuki Kasahara
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef ENABLE_PTHREAD
#include <pthread.h>
#endif

#include "eb.h"
#include "error.h"
#include "internal.h"


/*
 * Examine whether the current subbook in `book' supports `ENDWORD SEARCH'
 * or not.
 */
int
eb_have_endword_search(book)
    EB_Book *book;
{
    /*
     * Lock the book.
     */
    eb_lock(&book->lock);

    /*
     * Current subbook must have been set.
     */
    if (book->subbook_current == NULL)
	goto failed;

    /*
     * Check for the index page of endword search.
     */
    if (book->subbook_current->endword_alphabet.index_page == 0
	&& book->subbook_current->endword_asis.index_page == 0
	&& book->subbook_current->endword_kana.index_page == 0)
	goto failed;

    /*
     * Unlock the book.
     */
    eb_unlock(&book->lock);

    return 1;

    /*
     * An error occurs...
     */
  failed:
    eb_unlock(&book->lock);
    return 0;
}


/*
 * Endword search.
 */
EB_Error_Code
eb_search_endword(book, input_word)
    EB_Book *book;
    const char *input_word;
{
    EB_Error_Code error_code;
    EB_Word_Code word_code;
    EB_Search_Context *context;

    /*
     * Lock the book.
     */
    eb_lock(&book->lock);

    /*
     * Current subbook must have been set.
     */
    if (book->subbook_current == NULL) {
	error_code = EB_ERR_NO_CUR_SUB;
	goto failed;
    }

    /*
     * Initialize search context.
     */
    context = book->search_contexts;
    context->code = EB_SEARCH_ENDWORD;
    context->compare = eb_match_word;

    /*
     * Make a fixed word and a canonicalized word to search from
     * `input_word'.
     */
    error_code = eb_set_endword(book, input_word, context->word,
	context->canonicalized_word, &word_code);
    if (error_code != EB_SUCCESS)
	goto failed;

    /*
     * Get a page number.
     */
    switch (word_code) {
    case EB_WORD_ALPHABET:
	if (book->subbook_current->endword_alphabet.index_page != 0)
	    context->page = book->subbook_current->endword_alphabet.index_page;
	else if (book->subbook_current->endword_asis.index_page != 0)
	    context->page = book->subbook_current->endword_asis.index_page;
	else {
	    error_code = EB_ERR_NO_SUCH_SEARCH;
	    goto failed;
	}
	break;

    case EB_WORD_KANA:
	if (book->subbook_current->endword_kana.index_page != 0)
	    context->page = book->subbook_current->endword_kana.index_page;
	else if (book->subbook_current->endword_asis.index_page != 0)
	    context->page = book->subbook_current->endword_asis.index_page;
	else {
	    error_code = EB_ERR_NO_SUCH_SEARCH;
	    goto failed;
	}
	break;

    case EB_WORD_OTHER:
	if (book->subbook_current->endword_asis.index_page != 0)
	    context->page = book->subbook_current->endword_asis.index_page;
	else {
	    error_code = EB_ERR_NO_SUCH_SEARCH;
	    goto failed;
	}
	break;

    default:
	error_code = EB_ERR_NO_SUCH_SEARCH;
	goto failed;
    }

    /*
     * Pre-search.
     */
    error_code = eb_presearch_word(book, context);
    if (error_code != EB_SUCCESS)
	goto failed;

    /*
     * Unlock the book.
     */
    eb_unlock(&book->lock);

    return EB_SUCCESS;

    /*
     * An error occurs...
     */
  failed:
    book->search_contexts->code = EB_SEARCH_NONE;
    eb_unlock(&book->lock);
    return error_code;
}


