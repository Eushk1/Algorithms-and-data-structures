#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <IterativeMethod.h>
#include <Kinimatics.h>
#include <Arduino.h>
#include <random>
#include <cmath>
#include <vector>

// Структура для представления индивида (углы)
struct Individual {
    float angle1; // угол вектор1
    float angle2; // угол между вектор1 и вектор2
    float angle3; // угол между вектор2 и вектор3

    // Конструктор
    Individual(float a1, float a2, float a3) : angle1(a1), angle2(a2), angle3(a3) {}

    // Вычисление функции приспособленности
    float calculateFitness(float targetX, float targetY) const;

    // Функция для вычисления конечной точки
    void getEndPoint(float& x, float& y) const;
};



// Класс манипулятора
class Manipulator {
public:
    Manipulator() = default;

    // Публичный метод для получения состояния манипулятора
    ManipulatorState Gcode_command_G2(float targetX, float targetY,float targetZ);

private:
    // Приватные методы
    Individual generateRandomIndividual();
    Individual tournamentSelection(const std::vector<Individual>& population, float targetX, float targetY);
    Individual crossover(const Individual& parent1, const Individual& parent2);
    void mutate(Individual& individual);
    std::vector<Individual> geneticAlgorithm(float targetX, float targetY, int populationSize, int generations);
    void computeGradient(const Individual& individual, float targetX, float targetY, float& grad1, float& grad2, float& grad3);
    void gradientDescent(Individual& individual, float targetX, float targetY, float learningRate, int iterations);
};

#endif // MANIPULATOR_H

