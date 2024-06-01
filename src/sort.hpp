#ifndef SORT_HPP_
#define SORT_HPP_
#include <vector>
#include <cmath>
#include <functional>
namespace ticketsys
{
  template <class T, class Compare = std::less<T>>
  void Sort(std::vector<T> &A, int l, int r)
  {
    if (l >= r)
      return;
    int low = l, high = r;
    T key = A[low + rand() % (high - low + 1)];
    // T pivot = A[low];
    while (low <= high)
    {
      while (A[high] > key)
        high--;
      while (A[low] < key)
        low++;
      if (low <= high)
        std::swap(A[low++], A[high--]);
    }

    Sort(A, l, high);
    Sort(A, low, r);
  }
}
#endif