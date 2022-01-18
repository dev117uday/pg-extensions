# PgURI



```sql
\pset null _null_
\pset linestyle unicode
\pset border 2

CREATE DATABASE learn;

CREATE TABLE test (
       sno serial, link uri
);

INSERT INTO test (link)
VALUES ('http://www.postgresql.org/'),
       ('http://www.postgresql.org/docs/devel/#XFUNC-SQL-FUNCTION-ARGUMENTS'),
       ('http://www.postgresql.org:80/'),
       ('https://duckduckgo.com/?q=postgresql'),
       ('ftp://ftp.gnu.org/gnu/bison'),
       ('mailto:foo@example.com'),
       ('ssh://review.openstack.org:29418/openstack/nova.git'),
       ('http://admin:password@192.168.0.1'),
       ('http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]:80/index.html'),
       ('http://[1080::8:800:200C:417A]/foo');

SELECT sno,    
       uri_scheme(link),
       uri_userinfo(link),
       uri_host(link),
       uri_host_inet(link) 
FROM test;

SELECT sno,
       uri_port(link),
       uri_path(link),
       uri_path_array(link),
       uri_query(link),
       uri_fragment(link)
FROM test;
```



```s
rm -rf *.bc *.o *.so

make

sudo make install
```



