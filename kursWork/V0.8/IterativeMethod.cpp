#include <IterativeMethod.h>
#include <Kinimatics.h>
#include <Arduino.h>
#include <random>
#include <cmath>
#include <vector>


template <typename T>
T clamp(T value, T minValue, T maxValue) {
    return (value < minValue) ? minValue : (value > maxValue ? maxValue : value);
}

float Individual::calculateFitness(float targetX, float targetY) const {


    float x = hinge_1_len * cos(angle1 * M_PI / 180.0f) +
               hinge_2_len * cos((angle1 + angle2) * M_PI / 180.0f) +
               hinge_3_len * cos((angle1 + angle2 + angle3) * M_PI / 180.0f);
    float y = hinge_1_len * sin(angle1 * M_PI / 180.0f) +
               hinge_2_len * sin((angle1 + angle2) * M_PI / 180.0f) +
               hinge_3_len * sin((angle1 + angle2 + angle3) * M_PI / 180.0f);


    return 1.0f / (1.0f + sqrt(pow(x - targetX, 2) + pow(y - targetY, 2)));
}


void Individual::getEndPoint(float& x, float& y) const {
    x = hinge_1_len * cos(angle1 * M_PI / 180.0f) +
        hinge_2_len * cos((angle1 + angle2) * M_PI / 180.0f) +
        hinge_3_len * cos((angle1 + angle2 + angle3) * M_PI / 180.0f);

    y = hinge_1_len * sin(angle1 * M_PI / 180.0f) +
        hinge_2_len * sin((angle1 + angle2) * M_PI / 180.0f) +
        hinge_3_len * sin((angle1 + angle2 + angle3) * M_PI / 180.0f);
}




Individual Manipulator::generateRandomIndividual() {

    float angle1 = random(0, 18000001)/100000.0f;
    float angle2 = random(-13500000, 4500001)/100000.0f; 
    float angle3 = random(-13500000, 4500001)/100000.0f;

    return Individual(angle1, angle2, angle3);
}


Individual Manipulator::tournamentSelection(const std::vector<Individual>& population, float targetX, float targetY) {
    Individual best = population[random(0, population.size())]; 
    float bestFitness = best.calculateFitness(targetX, targetY);

    for (int i = 0; i < 3; ++i) { 
        Individual contender = population[random(0, population.size())];
        float contenderFitness = contender.calculateFitness(targetX, targetY);
        if (contenderFitness > bestFitness) {
            best = contender;
            bestFitness = contenderFitness;
        }
    }
    return best;
}


Individual Manipulator::crossover(const Individual& parent1, const Individual& parent2) {


    float randomValue1 = random(0, 1000001) / 1000000.0f; 
    float randomValue2 = random(0, 1000001) / 1000000.0f; 
    float randomValue3 = random(0, 1000001) / 1000000.0f; 

    return Individual(
        randomValue1 <  0.5f ? parent1.angle1 : parent2.angle1,
        randomValue2 <  0.5f ? parent1.angle2 : parent2.angle2,
        randomValue3 <  0.5f ? parent1.angle3 : parent2.angle3
    );
}


void Manipulator::mutate(Individual& individual) {

    individual.angle1 += random(-100000, +100001)/100000.0f; 
    individual.angle2 += random(-100000, +100001)/100000.0f;
    individual.angle3 += random(-100000, +100001)/100000.0f;


    individual.angle1 = fmod(individual.angle1 + 180.0f, 360.0f) - 180.0f;
    individual.angle2 = clamp(individual.angle2, -135.0f, 45.0f);
    individual.angle3 = clamp(individual.angle3, -135.0f, 45.0f);
}

std::vector<Individual> Manipulator::geneticAlgorithm(float targetX, float targetY, int populationSize, int generations) {
    std::vector<Individual> population;


    for (int i = 0; i < populationSize; ++i) {
        population.push_back(generateRandomIndividual());
    }

    for (int generation = 0; generation < generations; ++generation) {

        std::vector<Individual> newPopulation;


        for (int i = 0; i < populationSize; ++i) {
            Individual parent1 = tournamentSelection(population, targetX, targetY);
            Individual parent2 = tournamentSelection(population, targetX, targetY);
            Individual child = crossover(parent1, parent2);
            mutate(child);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }


    return population;
}


void Manipulator::computeGradient(const Individual& individual, float targetX, float targetY, float& grad1, float& grad2, float& grad3) {
    const float epsilon = 0.01f; 


    float fitnessCurrent = individual.calculateFitness(targetX, targetY);


    Individual tempIndividual = individual;


    tempIndividual.angle1 += epsilon;
    float fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad1 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;


    tempIndividual = individual;
    tempIndividual.angle2 += epsilon;
    fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad2 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;


    tempIndividual = individual;
    tempIndividual.angle3 += epsilon;
    fitnessPlusEpsilon = tempIndividual.calculateFitness(targetX, targetY);
    grad3 = (fitnessPlusEpsilon - fitnessCurrent) / epsilon;
}

void Manipulator::gradientDescent(Individual& individual, float targetX, float targetY, float learningRate, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        float grad1, grad2, grad3;
        computeGradient(individual, targetX, targetY, grad1, grad2, grad3);


        individual.angle1 += learningRate * grad1;
        individual.angle2 += learningRate * grad2;
        individual.angle3 += learningRate * grad3;


        individual.angle1 = fmod(individual.angle1 + 180.0f, 360.0f) - 180.0f; 
        individual.angle2 = clamp(individual.angle2, -135.0f, 45.0f);
        individual.angle3 = clamp(individual.angle3, -135.0f, 45.0f);
    }
}

ManipulatorState Manipulator::Gcode_command_G2(float targetX, float targetY, float targetZ) {
    ManipulatorState manipulatorstate_temp;
    int populationSize = 50;
    int generations = 150;


    manipulatorstate_temp.polarangle = atan2(targetY, targetX) * (180.0 / M_PI);


    float targetX_temp = sqrt(targetX * targetX + targetY * targetY);
    float targetY_temp = targetZ;


    std::vector<Individual> finalPopulation = geneticAlgorithm(targetX_temp, targetY_temp, populationSize, generations);


    Individual bestIndividual = finalPopulation[0];
    float bestFitness = bestIndividual.calculateFitness(targetX_temp, targetY_temp);
    for (const auto& individual : finalPopulation) {
      float fitness = individual.calculateFitness(targetX_temp, targetY_temp);
        if (fitness > bestFitness) {
            bestIndividual = individual;
            bestFitness = fitness;
        }
    }


    float learningRate = 0.01f; 
    int iterations = 100; 
    gradientDescent(bestIndividual, targetX_temp, targetY_temp, learningRate, iterations);


    manipulatorstate_temp.angle1 = bestIndividual.angle1;
    manipulatorstate_temp.angle2 = bestIndividual.angle2;
    manipulatorstate_temp.angle3 = bestIndividual.angle3;


    return manipulatorstate_temp;
}
