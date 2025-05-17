/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/

#ifndef CONTROLLER_H
    #define CONTROLLER_H
    #define LEFT (1 << 0)
    #define RIGHT (1 << 1)
    #define UP (1 << 2)
    #define DOWN (1 << 3)
    #include "call_csfml.h"
    #include "time.h"
typedef struct controller_s {
    sfUint8 control;
    time_management_t time;
}controller_t;

int check_cross(void);
int check_circle(void);
int check_square(void);
int check_triangle(void);
#endif //CONTROLLER_H
