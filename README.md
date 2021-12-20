# Learning about PostgreSQL Extension

## Requirements 
```s
sudo apt install postgresql-server-dev-13 gcc cmake build-essential
```

## Structure

- SQL Function should in `.sql` file with naming convention as `[name]--0.0.0.sql`
- Create a file `makefile` in project dir.

```makefile
EXTENSION = [name]        
DATA = [sql file name]
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
```

- Create a file named : `[name].control`

```
# [name] extension
comment = '[name] datatype'
default_version = '[version]'
relocatable = true
```

- Run `make install` to mount extension
- Connect to psql shell & run `CREATE EXTENSION [name];`