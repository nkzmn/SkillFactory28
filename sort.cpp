

#include <iostream>
#include <chrono>
#include <future>
#include <vector>

using namespace std;

std::mutex mut;

void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1; int n2 = r - m;

    // создаем временные массивы
    std::vector<int> L(n1);
    std::vector<int> R(n2);

    // копируем данные во временные массивы
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // слияние временных массивов в основной массив
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // копируем оставшиеся элементы из временных массивов, если такие есть
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (m - l > 100000) {
            // создаем фьючерсы для правой половины массива
            std::future<void> right = std::async(std::launch::async, mergeSort, std::ref(arr), m + 1, r);
            mergeSort(arr, l, m);
            // ждем завершения выполнения правой части массива
            right.get();
        }
        else
            mergeSort(arr, l, m);
            mergeSort(arr, m+1, r);
        // сливаем две отсортированные половины массива
            merge(arr, l, m, r);
    }
   
}



int main()
{
    string filename = "test.txt";
    const int size = 10000000;
    

    srand(time(nullptr)); // используем текущее время, чтобы сгенерировать рандомные значения
    int left_border = 1;
    int range_len = 10000; // правая граница = range_len + left_border
    
    vector <int> v;
    for (int i = 0; i < size; i++)
        v.push_back(left_border + rand() % range_len); // генерируем число в указанном диапазоне и записываем в массив


    

    auto start = chrono::high_resolution_clock::now(); // сохраняем время начала работы алгоритма
    

    /*for (auto it = v.begin(); it != v.end(); it++) {
        if (it != v.begin()) {
            std::cout << " ";
        }
        std::cout << *it;

    }
    cout << endl;*/

    mergeSort(v, 0, size-1);
    /*for (auto it = v.begin(); it != v.end(); it++) {
        if (it != v.begin()) {
            std::cout << " ";
        }
        std::cout << *it;

    }
    cout << endl;*/

    auto finish = chrono::high_resolution_clock::now(); // сохраняем время конца работы алгоритма
    chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " sec" << endl; // вычисляем продолжительность работы в сек. и выводим на экран

	return 0;
}