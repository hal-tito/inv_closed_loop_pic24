clearvars
clc

% Tri wave is also the carrier wave

% Parameters
fs = 20000;           % Switching frequency of carrier in Hz
f_ref = 50;          % Reference frequency in Hz
Vdc = 1;             % DC bus voltage for normalized output
t = 0:1e-6:0.04;     % Time vector for 40 ms

% Generate SPWM Reference and Carrier
ref1_wave = sin(2*pi*f_ref*t);
ref2_wave = sin(2*pi*f_ref*t + pi);

% Create a triangular carrier wave manually (since sawtooth may not exist or be inconsistent)
tri_wave = 2 * abs(2 * (fs * t - floor(fs * t + 0.5))) - 1;

% Generate SPWM signals
spwm1_signal = ref1_wave >= tri_wave;
spwm2_signal = ref2_wave >= tri_wave;

% Plot SPWM
figure;
subplot(2,1,1);
plot(t, ref1_wave, 'r', t, ref2_wave, 'g', t, tri_wave, 'b');
title('SPWM Reference and Carrier Wave');
xlabel('Time (s)'); ylabel('Amplitude');
legend('Sine 1 Reference', 'Sine 2 Reference', 'Triangular Carrier');
grid on;

subplot(2,1,2);
plot(t, spwm1_signal, 'r', t, spwm2_signal, 'b');
title('SPWM Output');
xlabel('Time (s)'); ylabel('PWM Signal');
legend('SPWM 1', 'SPWM 2');
ylim([-0.1 1.1]);
grid on;

