#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparison_count = 0;
long long max_recursion_depth = 0;
long long current_recursion_depth = 0;

void reset_metrics() {
    comparison_count = 0;
    max_recursion_depth = 0;
    current_recursion_depth = 0;
}

void merge(int arr[], int left, int mid, int right, int* temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        comparison_count++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort_internal(int arr[], int left, int right, int* temp) {
    current_recursion_depth++;
    if (current_recursion_depth > max_recursion_depth) {
        max_recursion_depth = current_recursion_depth;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_internal(arr, left, mid, temp);
        merge_sort_internal(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }

    current_recursion_depth--;
}

int partition_first(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;
    int temp;

    while (1) {
        while (i <= j) {
            comparison_count++;
            if (arr[i] <= pivot) {
                i++;
            }
            else {
                break;
            }
        }

        while (i <= j) {
            comparison_count++;
            if (arr[j] >= pivot) {
                j--;
            }
            else {
                break;
            }
        }

        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        else {
            break;
        }
    }

    temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}

void quick_sort_first_internal(int arr[], int low, int high) {
    current_recursion_depth++;
    if (current_recursion_depth > max_recursion_depth) {
        max_recursion_depth = current_recursion_depth;
    }

    if (low < high) {
        int pi = partition_first(arr, low, high);
        quick_sort_first_internal(arr, low, pi - 1);
        quick_sort_first_internal(arr, pi + 1, high);
    }

    current_recursion_depth--;
}

int partition_random(int arr[], int low, int high) {
    int rand_idx = low + rand() % (high - low + 1);
    int temp = arr[low];
    arr[low] = arr[rand_idx];
    arr[rand_idx] = temp;

    return partition_first(arr, low, high);
}

void quick_sort_random_internal(int arr[], int low, int high) {
    current_recursion_depth++;
    if (current_recursion_depth > max_recursion_depth) {
        max_recursion_depth = current_recursion_depth;
    }

    if (low < high) {
        int pi = partition_random(arr, low, high);
        quick_sort_random_internal(arr, low, pi - 1);
        quick_sort_random_internal(arr, pi + 1, high);
    }

    current_recursion_depth--;
}

int partition_median3(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    int idx1, idx2, med_idx;
    int temp;

    comparison_count++;
    if (arr[low] > arr[mid]) {
        idx1 = mid;
        idx2 = low;
    }
    else {
        idx1 = low;
        idx2 = mid;
    }

    comparison_count++;
    if (arr[idx2] > arr[high]) {
        comparison_count++;
        if (arr[idx1] > arr[high]) {
            med_idx = idx1;
        }
        else {
            med_idx = high;
        }
    }
    else {
        med_idx = idx2;
    }

    temp = arr[low];
    arr[low] = arr[med_idx];
    arr[med_idx] = temp;

    return partition_first(arr, low, high);
}

void quick_sort_median3_internal(int arr[], int low, int high) {
    current_recursion_depth++;
    if (current_recursion_depth > max_recursion_depth) {
        max_recursion_depth = current_recursion_depth;
    }

    if (low < high) {
        int pi = partition_median3(arr, low, high);
        quick_sort_median3_internal(arr, low, pi - 1);
        quick_sort_median3_internal(arr, pi + 1, high);
    }

    current_recursion_depth--;
}

int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    srand((unsigned int)time(NULL));
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* original = (int*)malloc(n * sizeof(int));
        int* data = (int*)malloc(n * sizeof(int));
        int* temp = (int*)malloc(n * sizeof(int));

        for (int i = 0; i < n; i++) {
            original[i] = rand() % 100000 + 1;
        }

        long long comp_merge = 0, comp_q1 = 0, comp_q2 = 0, comp_q3 = 0;
        double time_merge = 0, time_q1 = 0, time_q2 = 0, time_q3 = 0;
        size_t mem_merge = 0, mem_q1 = 0, mem_q2 = 0, mem_q3 = 0;

        clock_t start, end;

        for (int i = 0; i < n; i++) data[i] = original[i];
        reset_metrics();
        start = clock();
        merge_sort_internal(data, 0, n - 1, temp);
        end = clock();
        comp_merge = comparison_count;
        time_merge = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        mem_merge = (n * sizeof(int)) + (max_recursion_depth * 32);
        if (!is_sorted(data, n)) { printf("Merge Sort Error!\n"); }

        for (int i = 0; i < n; i++) data[i] = original[i];
        reset_metrics();
        start = clock();
        quick_sort_first_internal(data, 0, n - 1);
        end = clock();
        comp_q1 = comparison_count;
        time_q1 = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        mem_q1 = max_recursion_depth * 32;
        if (!is_sorted(data, n)) { printf("Quick First Error!\n"); }

        for (int i = 0; i < n; i++) data[i] = original[i];
        reset_metrics();
        start = clock();
        quick_sort_random_internal(data, 0, n - 1);
        end = clock();
        comp_q2 = comparison_count;
        time_q2 = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        mem_q2 = max_recursion_depth * 32;
        if (!is_sorted(data, n)) { printf("Quick Random Error!\n"); }

        for (int i = 0; i < n; i++) data[i] = original[i];
        reset_metrics();
        start = clock();
        quick_sort_median3_internal(data, 0, n - 1);
        end = clock();
        comp_q3 = comparison_count;
        time_q3 = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        mem_q3 = max_recursion_depth * 32;
        if (!is_sorted(data, n)) { printf("Quick Median3 Error!\n"); }

        printf("Data Size : %d\n\n", n);
        printf("Algorithm       Comparisons    Time(ms)    Memory(Bytes)\n");
        printf("--------------------------------------------------------\n");
        printf("Merge Sort      %-13lld %-11.4f %-13zu\n", comp_merge, time_merge, mem_merge);
        printf("Quick (First)   %-13lld %-11.4f %-13zu\n", comp_q1, time_q1, mem_q1);
        printf("Quick (Random)  %-13lld %-11.4f %-13zu\n", comp_q2, time_q2, mem_q2);
        printf("Quick (Median3) %-13lld %-11.4f %-13zu\n", comp_q3, time_q3, mem_q3);
        printf("\n");

        free(original);
        free(data);
        free(temp);
    }

    return 0;
}