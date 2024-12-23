#include <IterativeMethod.h>
#include <Kinimatics.h>
#include <Arduino.h>
#include <random>
#include <cmath>
#include <vector>

// Функция для ограничения значения в заданных пределах
template <typename T>
T clamp(T value, T minValue, T maxValue) {
    return (value < minValue) ? minValue : (value > maxValue ? maxValue : value);
}

// Функция для вычисления функции приспособленности
float Individual::calculateFitness(float targetX, float targetY) const {
    // Вычисляем координаты конца манипулятора
    float x = hinge_1_len * cos(angle1 * M_PI / 180.0f) +
               hinge_2_len * cos((angle1 + angle2) * M_PI / 180.0f) +
               hinge_3_len * cos((angle1 + angle2 + angle3) * M_PI / 180.0f);
    float y = hinge_1_len * sin(angle1 * M_PI / 180.0f) +
               hinge_2_len * sin((angle1 + angle2) * M_PI / 180.0f) +
               hinge_3_len * sin((angle1 + angle2 + angle3) * M_PI / 180.0f);

    // Функция приспособленности (минимизация расстояния до целевой точки)
    return 1.0f / (1.0f + sqrt(pow(x - targetX, 2) + pow(y - targetY, 2)));
}

// Функция для вычисления конечной точки
void Individual::getEndPoint(float& x, float& y) const {
    x = hinge_1_len * cos(angle1 * M_PI / 180.0f) +
        hinge_2_len * cos((angle1 + angle2) * M_PI / 180.0f) +
        hinge_3_len * cos((angle1 + angle2 + angle3) * M_PI / 180.0f);

    y = hinge_1_len * sin(angle1 * M_PI / 180.0f) +
        hinge_2_len * sin((angle1 + angle2) * M_PI / 180.0f) +
        hinge_3_len * sin((angle1 + angle2 + angle3) * M_PI / 180.0f);
}



// Генерация случайного индивида
Individual Manipulator::generateRandomIndividual() {
    // Используем random() для генерации случайных углов
    float angle1 = random(0, 18000001)/100000.0f; // угол1 от 0 до 180
    float angle2 = random(-13500000, 4500001)/100000.0f; // угол2 от -135 до 45
    float angle3 = random(-13500000, 4500001)/100000.0f; // угол3 от -135 до 45

    return Individual(angle1, angle2, angle3);
}

// Селекция по турниру
Individual Manipulator::tournamentSelection(const std::vector<Individual>& population, float targetX, float targetY) {
    Individual best = population[random(0, population.size())]; // Случайный выбор первого индивида
    float bestFitness = best.calculateFitness(targetX, targetY);

    for (int i = 0; i < 3; ++i) { // Турнир из 3-х участников
        Individual contender = population[random(0, population.size())];
        float contenderFitness = contender.calculateFitness(targetX, targetY);
        if (contenderFitness > bestFitness) {
            best = contender;
            bestFitness = contenderFitness;
        }
    }
    return best;
}

// Кроссовер
Individual Manipulator::crossover(const Individual& parent1, const Individual& parent2) {
    // Используем random() для выбора углов от родителей

    float randomValue1 = random(0, 1000001) / 1000000.0f; 
    float randomValue2 = random(0, 1000001) / 1000000.0f; 
    float randomValue3 = random(0, 1000001) / 1000000.0f; 

    return Individual(
        randomValue1 <  0.5f ? parent1.angle1 : parent2.angle1,
        randomValue2 <  0.5f ? parent1.angle2 : parent2.angle2,
        randomValue3 <  0.5f ? parent1.angle3 : parent2.angle3
    );
}

// Мутация
void Manipulator::mutate(Individual& individual) {
    // Изменение углов на случайное значение от -1 до 1
    individual.angle1 += random(-100000, +100001)/100000.0f; // Изменение углов на ±1 градус
    individual.angle2 += random(-100000, +100001)/100000.0f;
    individual.angle3 += random(-100000, +100001)/100000.0f;

    // Ограничение углов
    individual.angle1 = fmod(individual.angle1 + 180.0f, 360.0f) - 180.0f; // от 0 до 180
    individual.angle2 = clamp(individual.angle2, -135.0f, 45.0f);
    individual.angle3 = clamp(individual.angle3, -135.0f, 45.0f);
}
// Генетический алгоритм
std::vector<Individual> Manipulator::geneticAlgorithm(float targetX, float targetY, int populationSize, int generations) {
    std::vector<Individual> population;

    // Инициализация популяции
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(generateRandomIndividual());
    }

    for (int generation = 0; generation < generations; ++generation) {
        // Новая популяция
        std::vector<Individual> newPopulation;

        // Селекция и создание новой популяции
        for (int i = 0; i < populationSize; ++i) {
            Individual parent1 = tournamentSelection(population, targetX, targetY);
            Individual parent2 = tournamentSelection(population, targetX, targetY);
            Individual child = crossover(parent1, parent2);
            mutate(child);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }

    // Возвращаем лучшего индивида
    return population;
}

// Функция для вычисления градиента
void Manipulator::computeGradient(const Individual& individual, float targetX, float targetY, float& grad1, float& grad2, float& grad3) {
    const float epsilon = 0.01f; // малое значение для численного градиента

    // Вычисляем значение функции приспособленности для текущего индивидума
    float fitnessCurrent = individual.calculateFitness(targetX, targetY);

    // Вычисляем градиенты
    Individual tempIndividual = individual;

    // Градиент по angle1
    tempIndividual.angle1 += epsilon;
    float fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad1 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;

    // Градиент по angle2
    tempIndividual = individual;
    tempIndividual.angle2 += epsilon;
    fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad2 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;

    // Градиент по angle3
    tempIndividual = individual;
    tempIndividual.angle3 += epsilon;
    fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad3 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;
}
// Метод градиентного спуска
void Manipulator::gradientDescent(Individual& individual, float targetX, float targetY, float learningRate, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        float grad1, grad2, grad3;
        computeGradient(individual, targetX, targetY, grad1, grad2, grad3);

        // Обновляем углы
        individual.angle1 += learningRate * grad1;
        individual.angle2 += learningRate * grad2;
        individual.angle3 += learningRate * grad3;

        // Ограничение углов
        individual.angle1 = fmod(individual.angle1 + 180.0f, 360.0f) - 180.0f; 
        individual.angle2 = clamp(individual.angle2, -135.0f, 45.0f);
        individual.angle3 = clamp(individual.angle3, -135.0f, 45.0f);
    }
}

ManipulatorState Manipulator::Gcode_command_G2(float targetX, float targetY, float targetZ) {
    ManipulatorState manipulatorstate_temp;
    int populationSize = 50;
    int generations = 150;

    // Угол полярный
    manipulatorstate_temp.polarangle = atan2(targetY, targetX) * (180.0 / M_PI);

    // Расчет расстояния в плоскости XY
    float targetX_temp = sqrt(targetX * targetX + targetY * targetY);
    float targetY_temp = targetZ;

    // Запуск генетического алгоритма для получения финальной популяции
    std::vector<Individual> finalPopulation = geneticAlgorithm(targetX_temp, targetY_temp, populationSize, generations);

    // Поиск лучшего решения
    Individual bestIndividual = finalPopulation[0];
    float bestFitness = bestIndividual.calculateFitness(targetX_temp, targetY_temp);
    for (const auto& individual : finalPopulation) {
      float fitness = individual.calculateFitness(targetX_temp, targetY_temp);
        if (fitness > bestFitness) {
            bestIndividual = individual;
            bestFitness = fitness;
        }
    }

    // Применение градиентного спуска для улучшения решения
    float learningRate = 0.01f; // шаг обучения
    int iterations = 100; // количество итераций градиентного спуска
    gradientDescent(bestIndividual, targetX_temp, targetY_temp, learningRate, iterations);

    // Присвоение лучших углов состоянию манипулятора
    manipulatorstate_temp.angle1 = bestIndividual.angle1;
    manipulatorstate_temp.angle2 = bestIndividual.angle2;
    manipulatorstate_temp.angle3 = bestIndividual.angle3;


    return manipulatorstate_temp;
}