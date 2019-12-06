rng(42069); % for reproducible results

% constants and config
M = 4;
PULSE = 1;
symbol_bits = log2(M);

nums = 0:((M / 2) - 1);
p = (2.*nums + 1) .* PULSE;

map = [-flip(p) p];

L_b = 12;

input = make_input(L_b);

pad_size = mod(size(input, 1), symbol_bits);
input = [input zeros(pad_size)];

mapped = zeros([int32((size(input, 1) / symbol_bits)) 1]);

% convert input to symbols
for i = 1:size(mapped, 1)
    start = (i - 1) * symbol_bits;
    chunk = input((start + 1):(start + symbol_bits));
    
    num = binarray2dec(chunk);
    symbol = map(num + 1);
    mapped(i) = symbol;
end

fc = 2.5 * 10 ^ 6;
out = zeros([size(mapped) * 4 1]);
g_t = sqrt(2 / (0.4 * 10^-6));

for s = 1:size(mapped)
   ampl = mapped(s);
   for tick = 1:4 % tsample = 0.1μs, Tc = 0.4μs
       t = s * 0.4 * 10^-6 + tick * 0.1 * 10^-6; % current time seconds
       pulse = cos(2 * pi * 2.5 * 10 ^- 6 * t);
       
       out((s - 1) * 4 + tick) = ampl * g_t * pulse;
   end
end

noise = make_awgn(size(out));

received_raw = out + noise;

r = zeros(size(mapped));

% matched filter
for s = 1:size(mapped)
   h = zeros(4,1);
   filter_out = 0;
   
   start = s * 4;
   samples = received_raw(start:start+3);
   
   for tick = 1:4 % tsample = 0.1μs, Tc = 0.4μs
      t = s * 0.4 * 10^-6 + tick * 0.1 * 10^-6; % current time seconds
      pulse = cos(2 * pi * 2.5 * 10 ^- 6 * ((4 * 10^-6) -t));
       
      h(tick) = g_t * pulse;
   end
   
   res = conv(samples, h, 'same');
   r(s) = res(2); % 1st or?
end

received = zeros(size(mapped));
for s = 1:size(mapped) % decision device
   val = r(s);
   
   for i = 2:size(map)-2
       if val > (map(i-1) + map(i+1)) / 2 && val <= (map(i) + map(i+1)) / 2
           received(s) = map(i);
       end
   end
   
   if val < (map(1) + map(2)) / 2
       received(s) = map(1);
   end
   
   if val > (map(end) + map(end-1)) / 2
       received(s) = map(end);
   end
end

% SER

% BER


%%%
function out = make_input(length)
    out = rand([length 1]);
    out(out <= 0.5) = 0;
    out(out > 0.5) = 1;
end

function a = dec2binarray(i, digits)
    b = dec2bin(i, digits);
        
    bits = split(b, "");
    bits = cell2mat(bits(2:(end-1)));

    a = zeros(size(bits));
    a(bits == '1') = 1;
    a(bits == '0') = 0;
end

function a = binarray2dec(i)
    mappedbits = ['0'];
    mappedbits(i == 0) = '0';
    mappedbits(i == 1) = '1';
    
    a = bin2dec(mappedbits);
end

function noise = make_awgn(length)
    mu = 0;
    sigma = 0;
    noise = normrnd(mu, sigma, [length 1]);
end
