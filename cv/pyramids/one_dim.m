h = 1/16 * [1, 4, 6, 4, 1];

sample_pts = -36:1:37;

signal = my_signal(sample_pts);

num_pts = size(signal, 2);

g_0 = conv(signal, h, 'same'); % smooth pre subsample
g_1 = g_0(1:2:end);

l_1 = signal - expand_sig(g_1);
l_2 = g_1;

hold on
plot(sample_pts, signal);
%plot(sample_pts, g_0);
plot(sample_pts, expand_sig(g_1));
plot(sample_pts, l_1);
hold off

function signal = expand_sig(s)
    signal = interp(s, 2);
end

function signal = my_signal(x)
    % random polynomial or any func
    signal = sin(x) ./ x; 
end
