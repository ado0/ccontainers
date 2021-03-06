#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _STACK_MACRO_
#define _STACK_MACRO_

#define STACKSIZE 64

#define PREPARE_STACK                                                          \
        char *stack[STACKSIZE];                                                \
        char **stackptr = stack

#define PUSH(array, limit)                                                     \
        stackptr[0] = array;                                                   \
        stackptr[1] = limit;                                                   \
        stackptr += 2

#define POP(array, limit)                                                      \
        stackptr -= 2;                                                         \
        array = stackptr[0];                                                   \
        limit = stackptr[1]
#endif

#define THRESH 7

#define INIT_LIST(T)                                                           \
                                                                               \
        struct node_##T                                                        \
        {                                                                      \
                T data;                                                        \
                struct node_##T *next;                                         \
                struct node_##T *prev;                                         \
        };                                                                     \
                                                                               \
        struct list_##T                                                        \
        {                                                                      \
                size_t size;                                                   \
                struct node_##T *head;                                         \
                struct node_##T *tail;                                         \
                void (*copy)(T *, const T *);                                  \
                void (*free)(T *);                                             \
                int (*comp)(const void *, const void *);                       \
                int (*equ)(const T *, const T *);                              \
        };                                                                     \
                                                                               \
        static void flat_copy_##T(T *dst, const T *src)                        \
        {                                                                      \
                *dst = *src;                                                   \
        }                                                                      \
                                                                               \
        static int flat_equ_##T(const T *first, const T *second)               \
        {                                                                      \
                return memcmp(first, second, sizeof(T)) == 0;                  \
        }                                                                      \
                                                                               \
        static int flat_comp_##T(const void *first, const void *second)        \
        {                                                                      \
                return memcmp(first, second, sizeof(T));                       \
        }                                                                      \
                                                                               \
        size_t list_size_##T(const struct list_##T *l)                         \
        {                                                                      \
                return l->size;                                                \
        }                                                                      \
                                                                               \
        struct list_##T list_new_##T()                                         \
        {                                                                      \
                struct list_##T l = {                                          \
                    0,    NULL,          NULL,        flat_copy_##T,           \
                    NULL, flat_comp_##T, flat_equ_##T};                        \
                return l;                                                      \
        }                                                                      \
                                                                               \
        void list_set_copy_##T(struct list_##T *l,                             \
                               void (*copy)(T *, const T *))                   \
        {                                                                      \
                l->copy = copy;                                                \
        }                                                                      \
                                                                               \
        void list_set_free_##T(struct list_##T *l, void (*free_)(T *))         \
        {                                                                      \
                l->free = free_;                                               \
        }                                                                      \
                                                                               \
        void list_set_comp_##T(struct list_##T *l,                             \
                               int (*comp)(const void *, const void *))        \
        {                                                                      \
                l->comp = comp;                                                \
        }                                                                      \
                                                                               \
        void list_set_equ_##T(struct list_##T *l,                              \
                              int (*equ)(const T *, const T *))                \
        {                                                                      \
                l->equ = equ;                                                  \
        }                                                                      \
                                                                               \
        void list_init_##T(struct list_##T *l)                                 \
        {                                                                      \
                l->size = 0;                                                   \
                l->head = l->tail = NULL;                                      \
                l->copy = flat_copy_##T;                                       \
                l->free = NULL;                                                \
                l->comp = flat_comp_##T;                                       \
                l->equ = flat_equ_##T;                                         \
        }                                                                      \
                                                                               \
        void list_free_##T(struct list_##T *l)                                 \
        {                                                                      \
                struct node_##T *curr = l->head;                               \
                struct node_##T *tmp;                                          \
                for(size_t i = 0; i < l->size; ++i)                            \
                {                                                              \
                        tmp = curr;                                            \
                        curr = curr->next;                                     \
                        if(l->free)                                            \
                        {                                                      \
                                l->free(&tmp->data);                           \
                        }                                                      \
                        free(tmp);                                             \
                }                                                              \
                l->head = l->tail = NULL;                                      \
                l->size = 0;                                                   \
        }                                                                      \
                                                                               \
        int list_equ_##T(const struct list_##T *first,                         \
                         const struct list_##T *second)                        \
        {                                                                      \
                int equal = (first == second);                                 \
                if(first->equ == NULL)                                         \
                {                                                              \
                        fprintf(stderr, "UNDEFINED EQUALITY CONDITION\n");     \
                        return 0;                                              \
                }                                                              \
                if(equal == 0 && first->size == second->size)                  \
                {                                                              \
                        struct node_##T *curr_first = first->head;             \
                        struct node_##T *curr_second = second->head;           \
                        equal = 1;                                             \
                        while(curr_first)                                      \
                        {                                                      \
                                if(!first->equ(&curr_first->data,              \
                                               &curr_second->data))            \
                                {                                              \
                                        equal = 0;                             \
                                        break;                                 \
                                }                                              \
                                curr_first = curr_first->next;                 \
                                curr_second = curr_second->next;               \
                        }                                                      \
                }                                                              \
                return equal;                                                  \
        }                                                                      \
                                                                               \
        void list_copy_##T(struct list_##T *restrict dst,                      \
                           const struct list_##T *restrict src)                \
        {                                                                      \
                if(src->size != 0)                                             \
                {                                                              \
                        dst->head = (struct node_##T *)malloc(                 \
                            sizeof(struct node_##T));                          \
                        src->copy(&dst->head->data, &src->head->data);         \
                        struct node_##T *curr_src = src->head;                 \
                        struct node_##T *curr_dst = dst->head;                 \
                        struct node_##T *tmp_src = NULL;                       \
                        struct node_##T *tmp_dst = NULL;                       \
                        while(curr_src)                                        \
                        {                                                      \
                                tmp_src = curr_src->next;                      \
                                if(!tmp_src)                                   \
                                {                                              \
                                        break;                                 \
                                }                                              \
                                tmp_dst = (struct node_##T *)malloc(           \
                                    sizeof(struct node_##T));                  \
                                src->copy(&tmp_dst->data, &tmp_src->data);     \
                                tmp_dst->prev = curr_dst;                      \
                                curr_dst->next = tmp_dst;                      \
                                curr_dst = tmp_dst;                            \
                                curr_src = tmp_src;                            \
                        }                                                      \
                        dst->tail = curr_dst;                                  \
                        dst->tail->next = dst->head->prev = NULL;              \
                }                                                              \
                else                                                           \
                {                                                              \
                        dst->head = dst->tail = NULL;                          \
                }                                                              \
                dst->copy = src->copy;                                         \
                dst->free = src->free;                                         \
                dst->comp = src->comp;                                         \
                dst->equ = src->equ;                                           \
                dst->size = src->size;                                         \
        }                                                                      \
                                                                               \
        void list_push_back_##T(struct list_##T *l, T el)                      \
        {                                                                      \
                struct node_##T *new_el =                                      \
                    (struct node_##T *)malloc(sizeof(struct node_##T));        \
                l->copy(&new_el->data, &el);                                   \
                new_el->next = NULL;                                           \
                switch(l->size)                                                \
                {                                                              \
                case 0:                                                        \
                        new_el->prev = NULL;                                   \
                        l->head = l->tail = new_el;                            \
                        break;                                                 \
                case 1:                                                        \
                        l->tail = new_el;                                      \
                        l->head->next = l->tail;                               \
                        l->tail->prev = l->head;                               \
                        break;                                                 \
                default:                                                       \
                        new_el->prev = l->tail;                                \
                        l->tail->next = new_el;                                \
                        l->tail = l->tail->next;                               \
                        break;                                                 \
                }                                                              \
                l->size++;                                                     \
        }                                                                      \
                                                                               \
        T list_back_##T(const struct list_##T *l)                              \
        {                                                                      \
                if(l->size == 0)                                               \
                {                                                              \
                        fprintf(stderr,                                        \
                                "LIST_BACK ERROR: THE LIST IS EMPTY\n");       \
                        exit(1);                                               \
                }                                                              \
                else                                                           \
                {                                                              \
                        return l->tail->data;                                  \
                }                                                              \
        }                                                                      \
                                                                               \
        T list_pop_back_##T(struct list_##T *l)                                \
        {                                                                      \
                if(l->size == 0)                                               \
                {                                                              \
                        fprintf(stderr,                                        \
                                "POP_BACK ERROR: THE LIST IS EMPTY\n");        \
                        exit(1);                                               \
                }                                                              \
                else                                                           \
                {                                                              \
                        T ret = l->tail->data;                                 \
                        struct node_##T *tmp = l->tail;                        \
                        l->tail = l->tail->prev;                               \
                        l->free(&tmp->data);                                   \
                        free(tmp);                                             \
                        l->tail->next = NULL;                                  \
                        return ret;                                            \
                }                                                              \
        }                                                                      \
                                                                               \
        void list_push_front_##T(struct list_##T *l, const T el)               \
        {                                                                      \
                struct node_##T *new_el =                                      \
                    (struct node_##T *)malloc(sizeof(struct node_##T));        \
                l->copy(&new_el->data, &el);                                   \
                new_el->prev = NULL;                                           \
                switch(l->size)                                                \
                {                                                              \
                case 0:                                                        \
                        new_el->next = NULL;                                   \
                        l->head = l->tail = new_el;                            \
                        break;                                                 \
                case 1:                                                        \
                        l->head = new_el;                                      \
                        l->head->next = l->tail;                               \
                        l->tail->prev = l->head;                               \
                        break;                                                 \
                default:                                                       \
                        l->head->prev = new_el;                                \
                        new_el->next = l->head;                                \
                        l->head = l->head->prev;                               \
                        break;                                                 \
                }                                                              \
                l->size++;                                                     \
        }                                                                      \
                                                                               \
        T list_front_##T(const struct list_##T *l)                             \
        {                                                                      \
                if(l->size == 0)                                               \
                {                                                              \
                        fprintf(stderr,                                        \
                                "LIST_FRONT ERROR: THE LIST IS EMPTY\n");      \
                        exit(1);                                               \
                }                                                              \
                else                                                           \
                {                                                              \
                        return l->head->data;                                  \
                }                                                              \
        }                                                                      \
                                                                               \
        T list_pop_front_##T(struct list_##T *l)                               \
        {                                                                      \
                if(l->size == 0)                                               \
                {                                                              \
                        fprintf(stderr,                                        \
                                "POP_FRONT ERROR: THE LIST IS EMPTY\n");       \
                        exit(1);                                               \
                }                                                              \
                else                                                           \
                {                                                              \
                        struct node_##T *tmp = l->head;                        \
                        T ret = l->head->data;                                 \
                        l->head = l->head->next;                               \
                        l->free(&tmp->data);                                   \
                        free(tmp);                                             \
                        l->head->prev = NULL;                                  \
                        return ret;                                            \
                }                                                              \
        }                                                                      \
                                                                               \
        T list_at_##T(const struct list_##T *l, const size_t at)               \
        {                                                                      \
                if(at >= l->size)                                              \
                {                                                              \
                        fprintf(stderr, "LIST_AT ERROR: OUT OF RANGE\n");      \
                        exit(1);                                               \
                }                                                              \
                if(l->size == 0)                                               \
                {                                                              \
                        fprintf(stderr, "LIST_AT ERROR: THE LIST IS EMPTY\n"); \
                        exit(1);                                               \
                }                                                              \
                struct node_##T *curr = l->head;                               \
                for(size_t i = 0; i < at; ++i)                                 \
                {                                                              \
                        curr = curr->next;                                     \
                }                                                              \
                return curr->data;                                             \
        }                                                                      \
                                                                               \
        static struct node_##T *list_find_node_##T(const struct list_##T *l,   \
                                                   const T el)                 \
        {                                                                      \
                struct node_##T *location = NULL;                              \
                for(struct node_##T *curr = l->head; curr; curr = curr->next)  \
                {                                                              \
                        if(l->equ(&curr->data, &el))                           \
                        {                                                      \
                                location = curr;                               \
                                break;                                         \
                        }                                                      \
                }                                                              \
                return location;                                               \
        }                                                                      \
                                                                               \
        int list_find_##T(const struct list_##T *l, const T el)                \
        {                                                                      \
                struct node_##T *location = list_find_node_##T(l, el);         \
                return (location != NULL);                                     \
        }                                                                      \
                                                                               \
        void list_operate_##T(struct list_##T *l, void (*operate)(T *))        \
        {                                                                      \
                for(struct node_##T *curr = l->head; curr; curr = curr->next)  \
                {                                                              \
                        operate(&curr->data);                                  \
                }                                                              \
        }                                                                      \
                                                                               \
        void list_operate_to_##T(struct list_##T *l,                           \
                                 void (*operate)(T *, void *), void *argout)   \
        {                                                                      \
                for(struct node_##T *curr = l->head; curr; curr = curr->next)  \
                {                                                              \
                        operate(&curr->data, argout);                          \
                }                                                              \
        }                                                                      \
                                                                               \
        void list_operate_inverted_##T(struct list_##T *l,                     \
                                       void (*operate)(T *))                   \
        {                                                                      \
                for(struct node_##T *curr = l->tail; curr; curr = curr->prev)  \
                {                                                              \
                        operate(&curr->data);                                  \
                }                                                              \
        }                                                                      \
                                                                               \
        static void node_erase_##T(struct node_##T *n, void (*data_free)(T *)) \
        {                                                                      \
                if(n->next)                                                    \
                {                                                              \
                        n->next->prev = n->prev;                               \
                }                                                              \
                if(n->prev)                                                    \
                {                                                              \
                        n->prev->next = n->next;                               \
                }                                                              \
                data_free(&n->data);                                           \
                free(n);                                                       \
                n = NULL;                                                      \
        }                                                                      \
                                                                               \
        int list_erase_##T(struct list_##T *l, const T el)                     \
        {                                                                      \
                struct node_##T *n = list_find_node_##T(l, el);                \
                int erase = (n != NULL);                                       \
                if(erase)                                                      \
                {                                                              \
                        node_erase_##T(n, l->free);                            \
                        l->size--;                                             \
                }                                                              \
                return erase;                                                  \
        }                                                                      \
                                                                               \
        int list_erase_all_##T(struct list_##T *l, const T el)                 \
        {                                                                      \
                int erase = 0;                                                 \
                while(list_erase_##T(l, el))                                   \
                {                                                              \
                        erase = 1;                                             \
                }                                                              \
                return erase;                                                  \
        }                                                                      \
                                                                               \
        int list_erase_at_##T(struct list_##T *l, size_t at)                   \
        {                                                                      \
                int erase = (at - 1 < l->size && at != 0);                     \
                if(erase)                                                      \
                {                                                              \
                        struct node_##T *curr = l->head;                       \
                        for(size_t i = 0; i < at - 1; ++i)                     \
                        {                                                      \
                                curr = curr->next;                             \
                        }                                                      \
                        if(curr == l->head)                                    \
                        {                                                      \
                                l->head = l->head->next;                       \
                        }                                                      \
                        if(curr == l->tail)                                    \
                        {                                                      \
                                l->tail = l->tail->prev;                       \
                        }                                                      \
                        node_erase_##T(curr, l->free);                         \
                        l->size--;                                             \
                }                                                              \
                return erase;                                                  \
        }                                                                      \
                                                                               \
        int list_is_empty_##T(const struct list_##T *l)                        \
        {                                                                      \
                return (l->size == 0);                                         \
        }                                                                      \
                                                                               \
        static void list_insert_node_##T(struct node_##T *restrict curr,       \
                                         struct node_##T *restrict node_new)   \
        {                                                                      \
                struct node_##T *tmp = curr->next;                             \
                node_new->prev = curr;                                         \
                node_new->next = tmp;                                          \
                tmp->prev = node_new;                                          \
                curr->next = node_new;                                         \
        }                                                                      \
                                                                               \
        void list_insert_##T(struct list_##T *l, const size_t at, const T el)  \
        {                                                                      \
                if(at == 0)                                                    \
                {                                                              \
                        list_push_front_##T(l, el);                            \
                }                                                              \
                else if(at >= l->size)                                         \
                {                                                              \
                        list_push_back_##T(l, el);                             \
                }                                                              \
                else                                                           \
                {                                                              \
                        struct node_##T *new_node = (struct node_##T *)malloc( \
                            sizeof(struct node_##T));                          \
                        l->copy(&new_node->data, &el);                         \
                        struct node_##T *curr = l->head;                       \
                        for(size_t i = 0; i < at - 1; ++i)                     \
                        {                                                      \
                                curr = curr->next;                             \
                        }                                                      \
                        list_insert_node_##T(curr, new_node);                  \
                        l->size++;                                             \
                }                                                              \
        }                                                                      \
                                                                               \
        int list_is_sorted_##T(const struct list_##T *l,                       \
                               int (*comp)(const void *, const void *))        \
        {                                                                      \
                if(comp == NULL)                                               \
                {                                                              \
                        comp = l->comp;                                        \
                }                                                              \
                int sorted = 1;                                                \
                struct node_##T *curr = l->head;                               \
                struct node_##T *tmp;                                          \
                while(curr && sorted)                                          \
                {                                                              \
                        tmp = curr->next;                                      \
                        if(tmp && comp(&curr->data, &tmp->data))               \
                        {                                                      \
                                sorted = 0;                                    \
                        }                                                      \
                        curr = tmp;                                            \
                }                                                              \
                return sorted;                                                 \
        }                                                                      \
                                                                               \
        int list_insert_sorted_##T(struct list_##T *l, const T el)             \
        {                                                                      \
                int sorted = 1;                                                \
                if(l->head == NULL)                                            \
                {                                                              \
                        list_push_front_##T(l, el);                            \
                }                                                              \
                else if(l->tail && l->head != l->tail &&                       \
                        !l->comp(&l->tail->data, &l->head->data))              \
                {                                                              \
                        fprintf(stderr, "LIST_INSERT_SORTED ERROR: LIST NOT "  \
                                        "SORTED BY GIVEN CONDITION\n");        \
                        sorted = 0;                                            \
                }                                                              \
                else if((l->comp)(&l->head->data, &el))                        \
                {                                                              \
                        list_push_front_##T(l, el);                            \
                }                                                              \
                else if(!(l->comp)(&l->tail->data, &el))                       \
                {                                                              \
                        list_push_back_##T(l, el);                             \
                }                                                              \
                else                                                           \
                {                                                              \
                        struct node_##T *new_node = (struct node_##T *)malloc( \
                            sizeof(struct node_##T));                          \
                        l->copy(&new_node->data, &el);                         \
                        struct node_##T *curr = l->head;                       \
                        while(!l->comp(&curr->data, &el))                      \
                        {                                                      \
                                curr = curr->next;                             \
                        }                                                      \
                        list_insert_node_##T(curr->prev, new_node);            \
                        l->size++;                                             \
                }                                                              \
                return sorted;                                                 \
        }                                                                      \
                                                                               \
        static void list_memswp_##T(char *i, char *j)                          \
        {                                                                      \
                char tmp[sizeof(struct node_##T*)];                            \
                                                                               \
                memcpy(tmp, i, sizeof(struct node_##T *));                     \
                memcpy(i, j, sizeof(struct node_##T *));                       \
                memcpy(j, tmp, sizeof(struct node_##T *));                     \
        }                                                                      \
                                                                               \
        static inline int list_compare_node_##T(                               \
            const void *first, const void *second,                             \
            int comp(const void *, const void *))                              \
        {                                                                      \
                struct node_##T *first_ = *(struct node_##T **)first;          \
                struct node_##T *second_ = *(struct node_##T **)second;        \
                return comp(&first_->data, &second_->data);                    \
        }                                                                      \
                                                                               \
        static void list_qsort_##T(void *array, size_t array_size,             \
                                   int (*comp)(const void *, const void *))    \
        {                                                                      \
                char *i, *j;                                                   \
                size_t thresh = THRESH * sizeof(struct node_##T *);            \
                char *array_ = (char *)array;                                  \
                char *limit = array_ + array_size * sizeof(struct node_##T *); \
                PREPARE_STACK;                                                 \
                                                                               \
                while(1)                                                       \
                {                                                              \
                        if((size_t)(limit - array_) > thresh)                  \
                        {                                                      \
                                i = array_ + sizeof(struct node_##T *);        \
                                j = limit - sizeof(struct node_##T *);         \
                                list_memswp_##T(                               \
                                    ((((size_t)(limit - array_)) /             \
                                      sizeof(struct node_##T *)) /             \
                                     2) * sizeof(struct node_##T *) +          \
                                        array_,                                \
                                    array_);                                   \
                                if(list_compare_node_##T(i, j, comp) > 0)      \
                                {                                              \
                                        list_memswp_##T(i, j);                 \
                                }                                              \
                                if(list_compare_node_##T(array_, j, comp) > 0) \
                                {                                              \
                                        list_memswp_##T(array_, j);            \
                                }                                              \
                                if(list_compare_node_##T(i, array_, comp) > 0) \
                                {                                              \
                                        list_memswp_##T(i, array_);            \
                                }                                              \
                                while(1)                                       \
                                {                                              \
                                        do                                     \
                                        {                                      \
                                                i +=                           \
                                                    sizeof(struct node_##T *); \
                                        } while(list_compare_node_##T(         \
                                                    array_, i, comp) > 0);     \
                                        do                                     \
                                        {                                      \
                                                j -=                           \
                                                    sizeof(struct node_##T *); \
                                        } while(list_compare_node_##T(         \
                                                    j, array_, comp) > 0);     \
                                        if(i > j)                              \
                                        {                                      \
                                                break;                         \
                                        }                                      \
                                        list_memswp_##T(i, j);                 \
                                }                                              \
                                list_memswp_##T(array_, j);                    \
                                if(j - array_ > limit - i)                     \
                                {                                              \
                                        PUSH(array_, j);                       \
                                        array_ = i;                            \
                                }                                              \
                                else                                           \
                                {                                              \
                                        PUSH(i, limit);                        \
                                        limit = j;                             \
                                }                                              \
                        }                                                      \
                        else                                                   \
                        {                                                      \
                                for(j = array_,                                \
                                i = j + sizeof(struct node_##T *);             \
                                    i < limit;                                 \
                                    j = i, i += sizeof(struct node_##T *))     \
                                {                                              \
                                        for(;                                  \
                                            list_compare_node_##T(             \
                                                j,                             \
                                                j + sizeof(struct node_##T *), \
                                                comp) > 0;                     \
                                            j -= sizeof(struct node_##T *))    \
                                        {                                      \
                                                list_memswp_##T(               \
                                                    j,                         \
                                                    j + sizeof(struct node_##T \
                                                                   *));        \
                                                if(j == array_)                \
                                                {                              \
                                                        break;                 \
                                                }                              \
                                        }                                      \
                                }                                              \
                                if(stackptr != stack)                          \
                                {                                              \
                                        POP(array_, limit);                    \
                                }                                              \
                                else                                           \
                                {                                              \
                                        break;                                 \
                                }                                              \
                        }                                                      \
                }                                                              \
        }                                                                      \
                                                                               \
        static void ptr_array_to_list_##T(struct node_##T **nodes_ptr,         \
                                          struct list_##T *l)                  \
        {                                                                      \
                if(!l->size)                                                   \
                {                                                              \
                        return;                                                \
                }                                                              \
                                                                               \
                l->head = nodes_ptr[0];                                        \
                l->head->prev = NULL;                                          \
                                                                               \
                l->tail = nodes_ptr[l->size - 1];                              \
                l->tail->next = NULL;                                          \
                                                                               \
                if(l->size > 1)                                                \
                {                                                              \
                        l->head->next = nodes_ptr[1];                          \
                        l->tail->prev = nodes_ptr[l->size - 2];                \
                }                                                              \
                                                                               \
                for(size_t i = 1; i < l->size - 1; i++)                        \
                {                                                              \
                        nodes_ptr[i]->prev = nodes_ptr[i - 1];                 \
                        nodes_ptr[i]->next = nodes_ptr[i + 1];                 \
                }                                                              \
        }                                                                      \
                                                                               \
        void list_sort_##T(struct list_##T *l,                                 \
                           int (*comp)(const void *, const void *))            \
        {                                                                      \
                if(comp == NULL)                                               \
                {                                                              \
                        comp = l->comp;                                        \
                }                                                              \
                struct node_##T **nodes_ptr = (struct node_##T **)malloc(      \
                    sizeof(struct node_##T *) * l->size);                      \
                struct node_##T *curr = l->head;                               \
                                                                               \
                for(size_t i = 0; i < l->size; i++)                            \
                {                                                              \
                        nodes_ptr[i] = curr;                                   \
                        curr = curr->next;                                     \
                }                                                              \
                                                                               \
                list_qsort_##T(nodes_ptr, l->size, comp);                      \
                ptr_array_to_list_##T(nodes_ptr, l);                           \
                free(nodes_ptr);                                               \
        }\
