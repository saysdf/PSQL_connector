# PSQL_connector

Win32 build in zip file.
input arguments "[infile] [outfile] [dbname] [user] [password] [hostname] [port]" 


For install pqxx for vs use vspkg

clone from rep
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat


integrate vcpkg to visual studio 
> .\vcpkg.exe integrate install

install pqxx
> .\vcpkg.exe install libpqxx:x86-windows
