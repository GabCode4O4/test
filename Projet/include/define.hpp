#pragma once
#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

#include <iostream>
#include <vector>
#include <string>


typedef enum {
    COURS, 
    TP, 
    TD
}enseignement_t;

constexpr float COEF_TP = 2.f/3.f; 
constexpr float COEF_TD = 1.f;
constexpr float COEF_COURS = 1.5f;
constexpr float MAX_ETD_AUTRE_ENSEIGNANT = 384.f;
constexpr float MAX_ETD_ENSEIGNANT_CHERCHEUR = 192.f;
#endif