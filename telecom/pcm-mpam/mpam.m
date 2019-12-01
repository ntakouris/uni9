rng(42069); % for reproducible results

% constants and config
M = 4;

L_b = 24; % input length

R_symbols = 250 * 10^3; % 250Ksymbols / sec
T_symbol = 4 * 10 ^-6; % 4 μs

f_c = 2.5 * 10^6; % hz for transfer rate
T_c = 4 * 10 ^-7; % μs

E_s = 1; % symbol energy
E_b = E_s / log2(M); % bit energy

SNR = 10 * log10(1 / (2 * log2(M) * GAUSSIAN_S2);

% for demodulation assume coherency

% ----- Begin Simulation

input = make_input(L_b);
map_length = 4;

pad_size = mod(size(input), map_length);
input = padarray(input, pad_size); 

[mapper, demapper] = make_mapper(map_length, "normal");
symbol_length = log2(map_length);
mapped = zeros((size(input) / map_length) * symbol_length);

% convert to symbols
for i = 1:map_length:(size(input) - map_length + 1)
    chunk = input(i:(i+map_length));
    
    num = bi2de(chunk);
    symbol = mapper(num);
    
    mapped(i:size(symbol)) = symbol;
end

awgn = make_awgn(size(signal));


% pulses

signal = [];

received_raw = signal + awgn;
% coherent receive

% decision device
received_symbols = [];

demapped = zeros(size(received_symbols) * symbol_length);
% demapping
for i = 1:symbol_length:size(received_symbols)
    chunk = received_symbols(i:(i+symbol_length));
    
    num = bi2de(chunk);
    data = demapper(num);
    
    demapped(i:size(data)) = data;
end

received = demapped(1:(end - pad_size));

% random input
function out = make_input(length)
    out = rand([length 1]);
    out(input <= 0.5) = 0;
    out(input > 0.5) = 1;
end

% mapping
function [mapper, demapper] = make_mapper(length, type)
    mapper = zeros(length);
    
    for i = 0:(length-1)
        bits = dec2bi(i);
        mapper(i) = bits;
        
        if type == "gray"
            for j = 2:size(bits) % xor previous and next one, with the first one intact
                mapper(i,j) = mod((bits(j-1) + bits(j)), 2);
            end
        end
    end
    
    demapper = zeros(length);
    for i = 1:size(mapper)
        mapped = bi2dec(mapper(i));
        demapper(mapped) = dec2bi(i);
    end
end

% s_m -> g_t(t)


% -> cos(2 * pi * f_c * t)


% + AWGN -> r(t)
function noise = make_awgn(length)
    mu = 0;
    sigma = 1;
    noise = normrnd(mu, sigma, [length 1]);
end

% ==== Receiver ====

% symbol

% decision device

% demapping

% estimated_out
