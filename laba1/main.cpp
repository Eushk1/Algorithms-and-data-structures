#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <set>





using namespace std;
using namespace std::chrono;

long long globaln=0;

std::vector<int> generateRandomVector(int n) {
  std::vector<int> vec(n);
  for (int i = 1; i <= n; ++i) {
    vec[i - 1] = i;
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(vec.begin(), vec.end(), g);
  return vec;
}

std::vector<int> generateNearlySortedVector(int n) {
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = i + 1;
    }
        int numElementsToShuffle = n * 0.1;
        
        std::random_device rd;
        std::mt19937 g(rd());
        
        for (int i = 0; i < numElementsToShuffle; ++i) {
            int index1 = rand() % n;
            int index2 = rand() % n;
            std::swap(vec[index1], vec[index2]);
        }
    return vec;
}

std::vector<int> generateReverseSortedVector(int n) {
  std::vector<int> vec(n);
  for (int i = 0; i < n; ++i) {
    vec[i] = n - i;
  }
  return vec;
}

std::vector<int> generateSortedVector(int n) {
  std::vector<int> vec(n);
  for (int i = 0; i < n; ++i) {
    vec[i] = i+1;
  }
  return vec;
}




bool isSorted(std::vector<int>& arr, int low, int high) {
  for (int i = low; i < high - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
} //для богосорта

void shuffleArray(std::vector<int>& arr, int low, int high) {
  for (int i = low; i < high; i++) {
    int j = rand() % (high - low) + low;
    std::swap(arr[i], arr[j]);
  }
}  //для богосорта

void bogoSort(std::vector<int>& arr, int low, int high) {
  while (!isSorted(arr, low, high)) {
    shuffleArray(arr, low, high);
  }
}  //богосорт



void SelectionSort(std::vector<int>& arr, int low, int high) {
    for (int i = low; i < high - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < high; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}//Сортировка выбором

void InsertionSort(std::vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) { // Изменено < на <=
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}//сортировка вставками



void merge(std::vector<int>& arr, int low, int mid, int high) {
  std::vector<int> left(arr.begin() + low, arr.begin() + mid + 1);
  std::vector<int> right(arr.begin() + mid + 1, arr.begin() + high + 1);

  int i = 0, j = 0, k = low;

  while (i < left.size() && j < right.size()) {
    if (left[i] <= right[j]) {
      arr[k++] = left[i++];
    } else {
      arr[k++] = right[j++];
    }
  }

  while (i < left.size()) {
    arr[k++] = left[i++];
  }

  while (j < right.size()) {
    arr[k++] = right[j++];
  }
} //для сортировки слиянием


void mergeSort(std::vector<int>& arr, int low, int high) {
  if (low < high) {
    int mid = (low + high) / 2;

    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);

    merge(arr, low, mid, high);
  }
} //сортировка слиянием



int getNextShellGap(int gap) {
    return gap / 2;
}

void shellSortShell(std::vector<int>& arr) {
    int n = arr.size();
    int gap = n / 2;

    // Выполняем сортировку Шелла
    while (gap > 0) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap = getNextShellGap(gap);
    }
}//сортировка шелла шелл

int getNextHibbardGap(int gap) {
    return gap * 2 + 1;
}

void shellSortHibbard(std::vector<int>& arr) {
    int n = arr.size();
    int gap = 1;

    while (gap < n) {
        gap = getNextHibbardGap(gap);
    }
    gap = (gap - 1) / 2;

    while (gap > 0) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap = (gap - 1) / 2;
    }
}//сортировка шелла хиббард


std::vector<int> generatePrattGaps(int max_size) {
    std::set<int> sequence;
    max_size/=2;
    for (int i = 0; (1 << i) <= max_size; ++i) {
    int powerOfThree = 1;
        for (int j = 0; ; ++j) {
            int value = (1 << i) * powerOfThree;
            if (value > max_size) {
                break;
            }
            sequence.insert(value);
            powerOfThree *= 3;
        }
    }

    std::vector<int> result(sequence.begin(), sequence.end());
    return result;
}

void shellSortPratt(std::vector<int>& arr, const std::vector<int>& gaps) {
    int lenoflist = arr.size();
    for (int i = gaps.size() - 1; i >= 0; --i) {
        int gap = gaps[i];
        for (int j = gap; j < lenoflist; ++j) {
            int temp = arr[j];
            int k = j;
            while (k >= gap && arr[k - gap] > temp) {
                arr[k] = arr[k - gap];
                k -= gap;
            }
            arr[k] = temp;
        }
    }
}//сортировка шелла пратт


 


int partition(std::vector<int>& arr, int low, int high) {
    int randomIndex = low + std::rand() % (high - low + 1);
    std::swap(arr[randomIndex], arr[high]);
    int pivot = arr[high];
    int i = low - 1;
    globaln += 2;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
            globaln += 6;
        }
        globaln += 1;
    }

    std::swap(arr[i + 1], arr[high]);
    globaln += 5;
    return i + 1;
}

void quicksort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        globaln += 2;
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
} //быстрая сортировка


void bubbleSort(std::vector<int>& arr, int low, int high) {
  for (int i = low; i < high; i++) {
    for (int j = low; j < high - i; j++) {
      if (arr[j] > arr[j + 1]) {
          std::swap(arr[j], arr[j + 1]);
      }
    }
  }
} //сортивка пузырьком



void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
} //для сортировки кучей


void heapSort(std::vector<int>& arr, int low, int high) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i >= 0; --i) {
        std::swap(arr[0], arr[i]);

        heapify(arr, i, 0);
    }
} //сортировка кучей


int main() {
    int n = 10000;
   // int n =20;
    vector<int> array;
    
    fstream Out_Data;
    
    Out_Data.open("/Users/Egor 1/Desktop/Прога курсовик 3 сем/Лабы АиСД/АиСД лабораторные работы/Outdata.txt", ios::app);
    
    if (!Out_Data.is_open()) {
            cout << "Error opening file!";
            return 1;
        } else {
           cout << "File Open\n";
    }

    
    for(int l=0;l<12;l++){
        
        for(int i=0; i<25; i++){
            globaln=0;
        // array=generateSortedVector(n);
       //   array=generateReverseSortedVector(n);
            array=generateNearlySortedVector(n);
      // array=generateRandomVector(n);
            
            std::vector<int> gaps = generatePrattGaps(n);
            
            auto start = high_resolution_clock::now();
           // shellSortShell(array);
           //  shellSortHibbard(array);
            mergeSort(array, 0, array.size()-1);
         //    shellSortPratt(array, gaps);
   
            
            auto end = high_resolution_clock::now();
            
            auto duration = duration_cast<microseconds>(end - start);
            if(i>5)
            {
                cout<<"массив отсортирован:"<< is_sorted(array.begin(), array.end())<<" l: "<<l<<" размер массива: "<<static_cast<int>(array.size())<< " Затраченное время: " << duration.count() << " ms" << endl;
                
                Out_Data<<n<<" "<<duration.count()<<endl;
            }
            array.clear();
            
        }
        n=(10000*l)+20000;
        //n=pow(M_E, (0.7*(l+4)));
    }
    Out_Data.close();
    
    return 0;
}

