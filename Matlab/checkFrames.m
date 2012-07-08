close all;
step = 3855;
beg = 1;
stop = step;

plot(output1(beg:stop) - output2(beg:stop));
%figure(1);
% plot(output1(beg:stop));
% figure(2);
% plot(output2(beg:stop));
pause;

for i = 1:22
    %close all;
    beg = beg + step;
    stop = stop + step;
    plot(output1(beg:stop) - output2(beg:stop));
    %figure(1);
    %plot(output1(beg:stop));
    %figure(2);
    %plot(output2(beg:stop));
    pause;
end    