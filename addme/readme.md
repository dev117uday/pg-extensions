# Extension to ADD to INT

- From `addme` directory
    - to compile extension written in C : `make` 
    - to make the extensio : `make install`
- Connect to `psql`
    - run : `CREATE EXTENSION addme;`
    - example query : `SELECT addme(2,3);`