rng(42069); % for reproducible results

% constants and config
M = 4;

L_b = 30;

R_symbols = 250 * 10^3; % 250Ksymbols / sec
T_symbol = 4 * 10 ^-6; % 4 μs

MAPPER_CODE = "normal"; % or "gray"

f_c = 2.5 * 10^6; % hz for transfer rate
T_c = 4 * 10 ^-7; % μs

% simulate with 2 x Nyquist frequency

GAUSSIAN_M = 0; % μ
N_o = 1;
GAUSSIAN_S2 = N_o / 2; % σ ^ 2

E_s = 1; % symbol energy
E_b = E_s / log2(M); % bit energy

SNR = 10 * log10(1 / (2 * log2(M) * GAUSSIAN_S2);

% for demodulation assume coherency

% ----- Begin Simulation

% random input
function out = make_input(length)
    out = rand([length 1]);
    out(input <= 0.5) = 0;
    out(input > 0.5) = 1;
end

% mapping
function mapper = make_mapper(length, type)
    mapper = zeros(log2(length));
    
    for i = 0:(length-1)
        bits = dec2bi(i);
        mapper(i) = bits;
        
        if type == "gray"
            for j = 2:size(bits) % xor previous and next one, with the first one intact
                mapper(i,j) = mod((bits(j-1) + bits(j)), 2);
            end
        end
    end
end

% s_m -> g_t(t)


% -> cos(2 * pi * f_c * t)


% + AWGN -> r(t)

% ==== Receiver ====

% symbol

% decision device

% demapping

% estimated_out
