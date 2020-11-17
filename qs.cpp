#include <iostream>
#include <algorithm>
#include <set>
#include <fstream>
#include <time.h>
#include <iomanip>
#define MAX 31000

using namespace std;

struct timespec diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}
 
int PS2(int[], int, int, int);
int median(int[], int, int);

int PS2(int num[], int left, int right, int k) {
    if(right-left+1 <= 5) {
		// insert sort.
		int i, j, v;
        for(i = left+1; i <= right; i++) {
            v = num[i], j = i;
            while(j-1 >= left && num[j-1] > v)
                num[j] = num[j-1], j--;
            num[j] = v;
        }
	    /*int i, j, size;
		for (size = left + 1; size <= right; size++)
			for (i = size-1; i >= left && num[i-1] > num[i]; i--)
				swap(num[i-1], num[i]);*/
		//cout << left << " " << k << endl;
        return left+k;
    }
    int pivot = median(num, left, right);
    //cout << "pivot: " << pivot << endl;
    // partition begin.
    swap(num[left], num[pivot]);
    int i, j;
	int t = num[left];
    for(i = left, j = left+1; j <= right; j++) {
        if(num[j] <= t){
        	i++; 
			swap(num[i], num[j]);
		}
    }
    swap(num[left], num[i]);
    // partition end.
    int pos = i;
    if (pos == right) return pos;
    if(pos - left == k) return pos;
    if(pos - left > k)
    	return PS2(num, left, pos, k);
    else
        return PS2(num, pos+1, right, k-(pos-left+1));
}

int median(int num[], int left, int right) {
    int med = (right-left+4)/5;
    //cout << "med: " << med << endl;
    int i, subl, subr;
    int med_pos;
    for(i = 0; i < med; i++) {
        subl = left + i*5;
        subr = subl + 4;
        if(subr > right) subr = right;
        //cout << "subl: " << subl << "  subr: " << subr << endl;
        med_pos = PS2(num, subl, subr, (subr-subl)/2);
        //cout << "left+i: " << left+i << "  med_pos: " << med_pos << endl;
        swap(num[left+i], num[med_pos]);
    }
    //cout << "left: " << left << "  med: " << med << endl;
    return PS2(num, left, left+med, med/2);
}

void QS(int num[], int left, int right){ 
	if (left < right){
		int i = left;
		int j = right + 1;
		int pivot = num[left];
		do{
			do i++; while (num[i] < pivot);
			do j--; while (num[j] > pivot);
			if (i < j) swap(num[i], num[j]);
		} while (i < j);
		swap(num[left], num[j]);
		QS(num, left, j-1);
		QS(num, j+1, right);
	}
}

int main(){
	fstream file;
	int input_QS[MAX], input_PS[MAX];
	int ch, ans_QS, ans_PS, k_PS;
	int i = 0;
	
	file.open("input.txt", ios::in);
	file >> ch;
	int N = ch;
	file >> ch;
	int k = ch;
	while (file >> ch){
		input_QS[i] = ch;
		input_PS[i] = ch;
		i++;
	}
	file.close();
	
	struct timespec start, end, temp;
  	double time_QS, time_PS;
  	clock_gettime(CLOCK_MONOTONIC, &start);
	QS(input_QS, 0, N-1);
  	clock_gettime(CLOCK_MONOTONIC, &end);
  	temp = diff(start, end);
  	time_QS = temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;
  	  	
  	clock_gettime(CLOCK_MONOTONIC, &start);
  	k_PS = PS2(input_PS, 0, N-1, k-1);
  	clock_gettime(CLOCK_MONOTONIC, &end);
  	temp = diff(start, end);
  	time_PS = temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;
	
	ans_QS = input_QS[k-1];
	ans_PS = input_PS[k_PS];
	
	cout << ans_QS << endl;
	cout << fixed << setprecision(6) << time_QS << endl;
	cout << ans_PS << endl;
	cout << fixed << setprecision(6) << time_PS << endl;
	
	file.open("QS.txt", ios::out);
	file << ans_QS << endl;
	file << fixed << setprecision(6) << time_QS;
	file.close();
	file.open("PS.txt", ios::out);
	file << ans_PS << endl;
	file << fixed << setprecision(6) << time_PS;
	file.close();
	
	return 0;
} 
