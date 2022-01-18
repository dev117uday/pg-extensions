SET client_min_messages = warning;

CREATE TYPE uri;
CREATE CAST (uri AS text) WITH INOUT AS ASSIGNMENT;
CREATE CAST (text AS uri) WITH INOUT AS ASSIGNMENT;


CREATE FUNCTION uri_scheme(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_userinfo(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_host(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_host_inet(uri) RETURNS inet
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_port(uri) RETURNS integer
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_query(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_fragment(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_path(uri) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';

CREATE FUNCTION uri_path_array(uri) RETURNS text[]
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/uri';