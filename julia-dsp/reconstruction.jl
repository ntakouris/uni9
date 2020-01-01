using Plots
using DSP

### parameters
Ts = 0.02;
n = 0:(100 / Ts);

f0 = 5;

dt = 0.001;
t = 0:dt:10;

### sampling (the dot 'broadcasts' array operations | they are performed element-wise)
x = sin.(2 * pi * f0 * Ts * n); # target signal to sample | x ends up to be a vector of samples
println("Sampled")
# reconstructions #

### rectangular (functional projection from sampled signal to reconstructed)
# do not recompute length(_) on every iteration
length_t = length(t) 
ratio_n_t = length(n) / length(t)
ratio_t_n = length_t / length(n)

len_x = length(x)

rectangular_reconstr(i) = x[floor(Int, i * (ratio_n_t)) + 1] 
# this is called list comprehension
# it avoids iterator function calls
x_recon_rect = [rectangular_reconstr(e) for e in 1:(length_t-1)];
println("rectangular")

### triangular

triangle_reconstr(i) = begin
    low_i = floor(Int, i * (ratio_n_t)) + 1
    high_i = low_i + 1

    low_x = x[low_i]
    high_x = high_i > len_x ? low_x : x[high_i]
    
    return (i % ratio_t_n) * ((low_x + high_x) / 2) # interpolate samples 2-2
end

x_recon_tri = [triangle_reconstr(e) for e in 1:(length_t-1)];
println("triangular")

### sinc

_sinc(t) = begin
    n = floor(Int, t * (ratio_n_t) + 1)
    return [sinc((t - n * dt) / dt) for i in 1:length_t]
end

# for every produced sample, convolute start signal with shifted sinc
# could save some extra μs = 
x_recon_sinc = [conv(x, _sinc(i)) for i in 1:(length_t-1)];
println("sinc")

# plotting #
#pyplot() # plotting backend selection

# Plot all the reconstructions along with the original signal
#plot(x_recon_rect)
#plot!(x_recon_sinc)
#plot!(x_recon_tri)
