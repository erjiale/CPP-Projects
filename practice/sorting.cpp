#include <iostream>
#include <vector>
// #include <algorithm>

void insertionSort(std::vector<int> &arr)
{
    for (unsigned int i = 1; i < arr.size(); i++)
    {
        int j = i;
        while (j > 0 && arr[j] < arr[j - 1])
        {
            std::swap(arr[j], arr[j - 1]);
            j--;
        }
    }
    for (int i : arr)
    {
        std::cout << i << " ";
    }
    return;
}

int main()
{
    std::vector<int> myvector({9, 8, 5, 1, 3, 2});

    insertionSort(myvector);
    return 0;
}