#include <omp.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;
using namespace std;

void displayArray(string message, int nums[], int length)
{
    cout << "\t" << message << ": [";
    for (int i = 0; i < length; i++)
    {
        cout << nums[i];
        if (i != length - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

void merge(int nums[], int leftStart, int leftEnd, int rightStart, int rightEnd)
{
    int n = (rightEnd - leftStart) + 1; // Size of both arrays
    int tempArray[n];

    int t = 0;           // Index for temporary array
    int l = leftStart;   // Index for left array
    int r = rightStart;  // Index for right array

    // Merge both arrays into tempArray
    while (l <= leftEnd && r <= rightEnd)
    {
        if (nums[l] <= nums[r])
            tempArray[t++] = nums[l++];
        else
            tempArray[t++] = nums[r++];
    }

    // Copy remaining elements from left array
    while (l <= leftEnd)
        tempArray[t++] = nums[l++];

    // Copy remaining elements from right array
    while (r <= rightEnd)
        tempArray[t++] = nums[r++];

    // Copy back to original array
    for (int i = 0; i < n; i++)
        nums[leftStart + i] = tempArray[i];
}

void mergeSort(int nums[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
            mergeSort(nums, start, mid);
#pragma omp section
            mergeSort(nums, mid + 1, end);
        }
        merge(nums, start, mid, mid + 1, end);
    }
}
void seq_mergeSort(int nums[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;

        {

            seq_mergeSort(nums, start, mid);

            seq_mergeSort(nums, mid + 1, end);
        }
        merge(nums, start, mid, mid + 1, end);
    }
}

void bubbleSort(int nums[], int length)
{
    for (int i = 0; i < length; i++)
    {
        int start = i % 2; // Start from 0 if i is even else 1
#pragma omp parallel for
        for (int j = start; j < length - 1; j += 2)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}
void seq_bubbleSort(int nums[], int length)
{
    for (int i = 0; i < length; i++)
    {
        int start = i % 2; // Start from 0 if i is even else 1

        for (int j = start; j < length - 1; j += 2)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

int main()
{
    // Bubble Sort Example
    int nums1[] = {-925, -918, 869, -494, -505, 657, 688, -160, 747, -588, 542, 952, 366, 492, 860, -874, -913, -472, -217, -216, -638, -867, -504, -792, -567, 937, 25, -521, 427, -518, -940, -381, 411, -127, -751, 431, 49, 926, 258, -769, 751, 510, -597, -400, -851, 391, 96, 700, -455, -647, 653, -654, -521, -31, 873, -61, -549, -517, -57, 497, 133, 350, -285, 972, 997, 953, 199, 29, -11, 790, -412, -667, 204, 137, 117, -859, 723, 827, -708, -442, -812, -183, -617, -665, 22, -896, -107, -559, 537, -380, 457, -240, 83, -325, -926, -282, 5, 143, 442, 489};
    int length1 = sizeof(nums1) / sizeof(int);
    int nums2[] = {-925, -918, 869, -494, -505, 657, 688, -160, 747, -588, 542, 952, 366, 492, 860, -874, -913, -472, -217, -216, -638, -867, -504, -792, -567, 937, 25, -521, 427, -518, -940, -381, 411, -127, -751, 431, 49, 926, 258, -769, 751, 510, -597, -400, -851, 391, 96, 700, -455, -647, 653, -654, -521, -31, 873, -61, -549, -517, -57, 497, 133, 350, -285, 972, 997, 953, 199, 29, -11, 790, -412, -667, 204, 137, 117, -859, 723, 827, -708, -442, -812, -183, -617, -665, 22, -896, -107, -559, 537, -380, 457, -240, 83, -325, -926, -282, 5, 143, 442, 489};
    int length2 = sizeof(nums2) / sizeof(int);

    cout << "Bubble Sort:" << endl;
    displayArray("Before", nums1, length1);

    auto start_bubble = high_resolution_clock::now();
    bubbleSort(nums1, length1);
    auto end_bubble = high_resolution_clock::now();

    displayArray("After", nums1, length1);
    auto duration_bubble = duration_cast<microseconds>(end_bubble - start_bubble);
    cout << "\nExecution time for Bubble Sort: " << duration_bubble.count() << " microseconds" << endl;

    auto start_bubble_seq = high_resolution_clock::now();
    seq_bubbleSort(nums2, length2);
    auto end_bubble_seq = high_resolution_clock::now();

    //displayArray("After", nums1, length1);
    auto duration_bubble_seq = duration_cast<microseconds>(end_bubble_seq - start_bubble_seq);
    cout << "\nExecution time for Sequential Bubble Sort: " << duration_bubble_seq.count() << " microseconds" << endl;

    // Merge Sort Example
    int nums3[] = {-925, -918, 869, -494, -505, 657, 688, -160, 747, -588, 542, 952, 366, 492, 860, -874, -913, -472, -217, -216, -638, -867, -504, -792, -567, 937, 25, -521, 427, -518, -940, -381, 411, -127, -751, 431, 49, 926, 258, -769, 751, 510, -597, -400, -851, 391, 96, 700, -455, -647, 653, -654, -521, -31, 873, -61, -549, -517, -57, 497, 133, 350, -285, 972, 997, 953, 199, 29, -11, 790, -412, -667, 204, 137, 117, -859, 723, 827, -708, -442, -812, -183, -617, -665, 22, -896, -107, -559, 537, -380, 457, -240, 83, -325, -926, -282, 5, 143, 442, 489};
    int length3 = sizeof(nums3) / sizeof(int);
    int nums4[] = {-925, -918, 869, -494, -505, 657, 688, -160, 747, -588, 542, 952, 366, 492, 860, -874, -913, -472, -217, -216, -638, -867, -504, -792, -567, 937, 25, -521, 427, -518, -940, -381, 411, -127, -751, 431, 49, 926, 258, -769, 751, 510, -597, -400, -851, 391, 96, 700, -455, -647, 653, -654, -521, -31, 873, -61, -549, -517, -57, 497, 133, 350, -285, 972, 997, 953, 199, 29, -11, 790, -412, -667, 204, 137, 117, -859, 723, 827, -708, -442, -812, -183, -617, -665, 22, -896, -107, -559, 537, -380, 457, -240, 83, -325, -926, -282, 5, 143, 442, 489};
    int length4 = sizeof(nums4) / sizeof(int);

    cout << "\nMerge Sort:" << endl;
    displayArray("Before", nums3, length3);

    auto start_merge = high_resolution_clock::now();
    mergeSort(nums3, 0, length3 - 1);
    auto end_merge = high_resolution_clock::now();

    displayArray("After", nums3, length3);
    auto duration_merge = duration_cast<microseconds>(end_merge - start_merge);
    cout << "\nExecution time for Merge Sort: " << duration_merge.count() << " microseconds" << endl;

    auto start_merge_seq = high_resolution_clock::now();
    seq_mergeSort(nums4, 0, length4 - 1);
    auto end_merge_seq = high_resolution_clock::now();

   // displayArray("After", nums2, length2);
    auto duration_merge_seq = duration_cast<microseconds>(end_merge_seq - start_merge_seq);
    cout << "\nExecution time for Sequential Merge Sort: " << duration_merge_seq.count() << " microseconds" << endl;

    return 0;
}
