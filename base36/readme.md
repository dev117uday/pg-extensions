# Extension to convert INT to base36

- From inside the directory, run : `make install`
- Connect to psql shell 
    - run : `CREATE EXTENSION base36;`
    - example : `SELECT base36_encode(123456);`

