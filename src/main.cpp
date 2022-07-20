#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <emscripten.h>

typedef struct {
    float *a;
    float *b;
    float *c;
    int m;
    int n;
    int k;
    int mStart;
    int mEnd;
} MatMulInfo;

void* matmul(void* info_);

float *data_a = nullptr, *data_b = nullptr, *data_c = nullptr;

extern "C" EMSCRIPTEN_KEEPALIVE
void do_alloc(int m, int n, int k) {
    data_a = new float[m*k];
    data_b = new float[k*n];
    data_c = new float[m*n];
}

extern "C" EMSCRIPTEN_KEEPALIVE
float* get_a() {
    return data_a;
}

extern "C" EMSCRIPTEN_KEEPALIVE
float* get_b() {
    return data_b;
}

extern "C" EMSCRIPTEN_KEEPALIVE
float* get_c() {
    return data_c;
}

extern "C" EMSCRIPTEN_KEEPALIVE
void compute(int m, int n, int k, int parallel) {
    if (parallel == 0) {
        MatMulInfo info = {data_a, data_b, data_c, m, n, k, 0, m};
        matmul(&info);
    } else {
        pthread_t *threads = new pthread_t[parallel];
        MatMulInfo *infos = new MatMulInfo[parallel];
        int slice = m / parallel;
        // parallel > PTHREAD_POOL_SIZE とするとデッドロックするので注意
        for (int i = 0; i < parallel; i++) {
            infos[i] = MatMulInfo{data_a, data_b, data_c, m, n, k, slice*i, slice*(i+1)};
            int ret;
            ret = pthread_create(&threads[i], NULL, matmul, &infos[i]);
            if (ret != 0) {
                printf("pthread_create failed %d\n", ret);
                return;
            }
        }
        printf("pthread_create ok\n");
        for (int i = 0; i < parallel; i++) {
            int ret = pthread_join(threads[i], NULL);
            if (ret != 0) {
                printf("pthread_join failed %d\n", ret);
                return;
            }
        }
    }
    printf("compute end\n");
}

void* matmul(void* info_) {
    printf("matmul thread\n");
    MatMulInfo* info = (MatMulInfo*)info_;
    for (int mi = info->mStart; mi < info->mEnd; mi++) {
        for (int ni = 0; ni < info->n; ni++) {
            float s = 0.0;
            for (int ki = 0; ki < info->k; ki++) {
                s += info->a[mi * info->k + ki] * info->b[ki * info->n + ni];
            }
            info->c[mi * info->n + ni] = s;
        }
    }
    return nullptr;
}
