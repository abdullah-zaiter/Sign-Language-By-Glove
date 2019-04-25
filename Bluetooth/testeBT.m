%creat a bluetooth object
%HC-05 channel default is 1
bt = Bluetooth('HC-05',1);
fopen(bt);
%write and read function
fwrite(bt,1,'uchar'); % pede para enviar dados da mao 1
x=fread(bt,5,'uchar'); % recebe 5 dados

fwrite(bt,2,'uchar'); % pede para enviar dados da mao 2
y=fread(bt,5,'uchar'); % recebe 5 dados

fclose(bt);
