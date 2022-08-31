filepath = './Traces/Traces_00000.dat';

fp = fopen(filepath, 'rb');

not = fread(fp, 1, 'uint32')

for i = 1:not
    nop = fread(fp, 1, 'uint32');
    if (i == 1)
        traces = zeros(not, nop);
        plains = zeros(not, 16);
        ciphers = zeros(not, 16);
    end
    traces(i, :) = fread(fp, nop, 'int8');
    plains(i,:) = fread(fp, 16, 'uint8');
    ciphers(i,:) = fread(fp, 16, 'uint8');
    %dummy = fread(fp, 4, 'uint8');
end
fclose(fp);