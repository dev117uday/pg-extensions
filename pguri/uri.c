#include <postgres.h>
#include <access/hash.h>
#include <catalog/pg_type.h>
#include <fmgr.h>
#include <lib/stringinfo.h>
#include <utils/array.h>
#include <utils/builtins.h>
#include <utils/inet.h>

#include <uriparser/Uri.h>

static void parse_uri(const char *s, UriUriA *urip){
	UriParserStateA state;

	state.uri = urip;
	uriParseUriA(&state, s);

	switch (state.errorCode)
	{
		case URI_SUCCESS:
			return;
		case URI_ERROR_SYNTAX:
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					 errmsg("invalid input syntax for type uri at or near \"%s\"",
							state.errorPos)));
			break;
		default:
			elog(ERROR, "liburiparser error code %d", state.errorCode);
	}
}

static text * uri_text_range_to_text(UriTextRangeA r){
	if (!r.first || !r.afterLast)
		return NULL;

	return cstring_to_text_with_len(r.first, r.afterLast - r.first);
}

//done
PG_FUNCTION_INFO_V1(uri_scheme);
Datum uri_scheme(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	text *result;

	parse_uri(s, &uri);
	result = uri_text_range_to_text(uri.scheme);
	uriFreeUriMembersA(&uri);
	if (result)
		PG_RETURN_TEXT_P(result);
	else
		PG_RETURN_NULL();
}


// done
PG_FUNCTION_INFO_V1(uri_userinfo);
Datum uri_userinfo(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	text *result;

	parse_uri(s, &uri);
	result = uri_text_range_to_text(uri.userInfo);
	uriFreeUriMembersA(&uri);
	if (result)
		PG_RETURN_TEXT_P(result);
	else
		PG_RETURN_NULL();
}


// done
PG_FUNCTION_INFO_V1(uri_host);
Datum uri_host(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	text *result;

	parse_uri(s, &uri);
	result = uri_text_range_to_text(uri.hostText);
	uriFreeUriMembersA(&uri);
	if (result)
		PG_RETURN_TEXT_P(result);
	else
		PG_RETURN_NULL();
}


// done
PG_FUNCTION_INFO_V1(uri_host_inet);
Datum uri_host_inet(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;

	parse_uri(s, &uri);
	if (uri.hostData.ip4)
	{
		unsigned char *data = uri.hostData.ip4->data;
		char *tmp = palloc(16);
		snprintf(tmp, 16, "%u.%u.%u.%u", data[0], data[1], data[2], data[3]);
		uriFreeUriMembersA(&uri);
		PG_RETURN_INET_P(DirectFunctionCall1(inet_in, CStringGetDatum(tmp)));
	}
	else if (uri.hostData.ip6)
	{
		unsigned char *data = uri.hostData.ip6->data;
		char *tmp = palloc(40);
		snprintf(tmp, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
				 data[0], data[1], data[2], data[3],
				 data[4], data[5], data[6], data[7],
				 data[8], data[9], data[10], data[11],
				 data[12], data[13], data[14], data[15]);
		uriFreeUriMembersA(&uri);
		PG_RETURN_INET_P(DirectFunctionCall1(inet_in, CStringGetDatum(tmp)));
	}
	else
	{
		uriFreeUriMembersA(&uri);
		PG_RETURN_NULL();
	}
}

// done
static int _uri_port_num(UriUriA *urip)
{
	if (!urip->portText.first || !urip->portText.afterLast
		|| urip->portText.afterLast == urip->portText.first)
		return -1;
	return strtol(pnstrdup(urip->portText.first, urip->portText.afterLast - urip->portText.first),
				 NULL, 10);
}

// done
PG_FUNCTION_INFO_V1(uri_port);
Datum uri_port(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	int num;

	parse_uri(s, &uri);
	num = _uri_port_num(&uri);
	uriFreeUriMembersA(&uri);
	if (num < 0)
		PG_RETURN_NULL();
	PG_RETURN_INT32(num);
}

// done
PG_FUNCTION_INFO_V1(uri_query);
Datum uri_query(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	text *result;

	parse_uri(s, &uri);
	result = uri_text_range_to_text(uri.query);
	uriFreeUriMembersA(&uri);
	if (result)
		PG_RETURN_TEXT_P(result);
	else
		PG_RETURN_NULL();
}

// done
PG_FUNCTION_INFO_V1(uri_fragment);
Datum uri_fragment(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	text *result;

	parse_uri(s, &uri);
	result = uri_text_range_to_text(uri.fragment);
	uriFreeUriMembersA(&uri);
	if (result)
		PG_RETURN_TEXT_P(result);
	else
		PG_RETURN_NULL();
}

static bool _is_host_set(UriUriA *uri)
{
	return (uri != NULL)
		&& ((uri->hostText.first != NULL)
			|| (uri->hostData.ip4 != NULL)
			|| (uri->hostData.ip6 != NULL)
			|| (uri->hostData.ipFuture.first != NULL)
			);
}

// done
PG_FUNCTION_INFO_V1(uri_path);
Datum uri_path(PG_FUNCTION_ARGS)
{
	Datum arg = PG_GETARG_DATUM(0);
	char *s = TextDatumGetCString(arg);
	UriUriA uri;
	StringInfoData buf;
	UriPathSegmentA *p;

	initStringInfo(&buf);

	parse_uri(s, &uri);

	if (uri.absolutePath || (_is_host_set(&uri) && uri.pathHead))
		appendStringInfoChar(&buf, '/');

	for (p = uri.pathHead; p; p = p->next)
	{
		appendBinaryStringInfo(&buf, p->text.first, p->text.afterLast - p->text.first);
		if (p->next)
			appendStringInfoChar(&buf, '/');
	}

	uriFreeUriMembersA(&uri);
	PG_RETURN_TEXT_P(cstring_to_text(buf.data));
}