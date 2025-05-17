/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/

#ifndef TIME_H
    #define TIME_H
    #include "time.h"
    #define SS 2
    #define MUTANT 3
    #define BRUT 4

typedef struct time_management_s {
    sfClock *clock;
    sfTime time;
    sfInt64 time_as_micro_second;
    float time_as_second;
}time_management_t;

#endif //TIME_H
