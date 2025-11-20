#include <iostream>
#include <cassert>
#include <omp.h>

#define NO_CUDA
#include "util.h"

#ifdef _OPENACC
    #include <openacc.h>
#endif

double blur(int pos, const double *u)
{
    return 0.25*(u[pos-1] + 2.0*u[pos] + u[pos+1]);
}

void print_buffer(double *buf, size_t n, const char *msg)
{
    std::cout << msg << "@" << buf << ": ";
    for (auto i = 0; i < n; ++i) {
        std::cout << buf[i] << " ";
    }
    std::cout << "\n";
}

void blur_twice_host(double *in , double *out , int n, int nsteps)
{
    double *buffer = malloc_host<double>(n);
    for (auto istep = 0; istep < nsteps; ++istep) {
        #pragma omp parallel for
        for (auto i = 1; i < n-1; ++i) {
            buffer[i] = blur(i, in);
        }

        #pragma omp parallel for
        for(auto i = 2; i < n-2; ++i) {
            out[i] = blur(i, buffer);
        }

        in = out;
    }
    free(buffer);
}

void blur_twice_gpu_naive(double *in , double *out , int n, int nsteps)
{
    double *buffer = malloc_host<double>(n);

    for (auto istep = 0; istep < nsteps; ++istep) {
        #pragma acc parallel loop copyin(in[0:n]) copyout(buffer[0:n])
        for (auto i = 1; i < n-1; ++i) {
            buffer[i] = blur(i, in);
        }

        #pragma acc parallel loop copyin(buffer[0:n]) copyout(out[0:n])
        for (auto i = 2; i < n-2; ++i) {
            out[i] = blur(i, buffer);
        }

        // Copy out to host
        for (auto i = 0; i < n; ++i) {
            in[i] = out[i];
        }
    }

    free(buffer);
}

void blur_twice_gpu_nocopies(double *in , double *out , int n, int nsteps)
{
    double *buffer = malloc_host<double>(n);

    #pragma acc data copyin(in[0:n]) copyout(out[0:n]) create(buffer[0:n])
    {
        for (auto istep = 0; istep < nsteps; ++istep) {
            #pragma acc parallel loop
            for (auto i = 1; i < n-1; ++i) {
                buffer[i] = blur(i, in);
            }

            #pragma acc parallel loop
            for (auto i = 2; i < n-2; ++i) {
                out[i] = blur(i, buffer);
            }

            // Simple pointer copy on device
            #pragma acc parallel loop
            for (auto i = 0; i < n; ++i) {
                in[i] = out[i];
            }
        }
    }

    free(buffer);
}

int main(int argc, char** argv) {
    size_t pow    = read_arg(argc, argv, 1, 20);
    size_t nsteps = read_arg(argc, argv, 2, 100);
    size_t n = (1 << pow) + 4;

    auto size_in_bytes = n * sizeof(double);
    std::cout << "dispersion 1D test of length n = " << n
              << " : " << size_in_bytes/(1024.*1024.) << "MB\n";

    auto x0_orig = malloc_host<double>(n, 0.);
    auto x1_orig = malloc_host<double>(n, 0.);
    auto x0 = malloc_host<double>(n, 0.);
    auto x1 = malloc_host<double>(n, 0.);
    auto x0_nc = malloc_host<double>(n, 0.);
    auto x1_nc = malloc_host<double>(n, 0.);

    // set boundary conditions to 1
    x0[0] = x1[0] = x0_orig[0] = x1_orig[0] = 1.0;
    x0[1] = x1[1] = x0_orig[1] = x1_orig[1] = 1.0;
    x0[n-2] = x1[n-2] = x0_orig[n-2] = x1_orig[n-2] = 1.0;
    x0[n-1] = x1[n-1] = x0_orig[n-1] = x1_orig[n-1] = 1.0;
    x0_nc[0] = x1_nc[0] = x0_orig[0];
    x0_nc[1] = x1_nc[1] = x0_orig[1];
    x0_nc[n-2] = x1_nc[n-2] = x0_orig[n-2];
    x0_nc[n-1] = x1_nc[n-1] = x0_orig[n-1];

    auto tstart_host = get_time();
    blur_twice_host(x0_orig, x1_orig, n, nsteps);
    auto time_host = get_time() - tstart_host;

    auto tstart_gpu = get_time();
    blur_twice_gpu_naive(x0, x1, n, nsteps);
    auto time_gpu = get_time() - tstart_gpu;

    auto tstart_nc = get_time();
    blur_twice_gpu_nocopies(x0_nc, x1_nc, n, nsteps);
    auto time_nc = get_time() - tstart_nc;

    auto validate_naive = true;
    auto validate_nc = true;
    for (auto i = 0; i < n; ++i) {
        if (std::abs(x1_orig[i] - x1[i]) > 1.e-6) validate_naive = false;
        if (std::abs(x1_orig[i] - x1_nc[i]) > 1.e-6) validate_nc = false;
    }

    std::cout << "==== Validation: host vs GPU naive: " << (validate_naive ? "success" : "failure") << " ====\n";
    std::cout << "==== Validation: host vs GPU no copies: " << (validate_nc ? "success" : "failure") << " ====\n";

    std::cout << "Host version took " << time_host << " s"
              << " (" << time_host/nsteps << " s/step)\n";
    std::cout << "GPU naive version took " << time_gpu << " s"
              << " (" << time_gpu/nsteps << " s/step)\n";
    std::cout << "GPU no-copies version took " << time_nc << " s"
              << " (" << time_nc/nsteps << " s/step)\n";

    return 0;
}
