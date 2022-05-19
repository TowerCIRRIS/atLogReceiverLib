clear; clc; close all

loadlibrary('AtlogReceiver.dll','AtlogReceiver.h');
libfunctions AtlogReceiver -full


port = 5;       %Num�ro du port s�rie
baud = 460800;  %Son baud rate

% Ouverture du port s�rie
result =  calllib('AtlogReceiver','openComm', port, baud)

if result
    
    % Rafraichir la liste de fichier disponibles sur l'appareil
    fileCount = calllib('AtlogReceiver','refreshFileList')

    if (fileCount)
        
        filename = "";
        for i=0:(fileCount-1)

            % T�l�chargement du fichier choisi i = la position dans
            % l'index, pas le num�ro dans le nom du fichier.  
            result = calllib('AtlogReceiver','downloadDataByNumber',i)

            %Optionnel pour afficher le nom du fichi� t�l�charg�
            if(result)
                [retval,filename] = calllib('AtlogReceiver','getFileName',i,blanks(200));
                filename
            end
        end
    end
end


% Close serial port
calllib('AtlogReceiver','endComm')

unloadlibrary('AtlogReceiver') 

%Exemple autres fonctions

% Pour t�l�charger un fichier en utilisant son nom plutot que sa position
% dans la liste.
% result =  calllib('AtlogReceiver','downloadDataByName','Data0001.dat')

% filecount = calllib('AtlogReceiver','getFileListCount')
%calllib('AtlogReceiver','startLog')
%calllib('AtlogReceiver','stopLog')
%calllib('AtlogReceiver','deleteAllLog')