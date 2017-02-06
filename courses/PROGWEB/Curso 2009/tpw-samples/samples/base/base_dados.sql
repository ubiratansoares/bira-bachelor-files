--
-- PostgreSQL database dump
--

SET client_encoding = 'SQL_ASCII';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'Standard public schema';


SET search_path = public, pg_catalog;

--
-- Name: _news_seq; Type: SEQUENCE; Schema: public; Owner: usernews
--

CREATE SEQUENCE _news_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public._news_seq OWNER TO usernews;

--
-- Name: _news_seq; Type: SEQUENCE SET; Schema: public; Owner: usernews
--

SELECT pg_catalog.setval('_news_seq', 6, true);


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: _news; Type: TABLE; Schema: public; Owner: usernews; Tablespace: 
--

CREATE TABLE _news (
    code integer DEFAULT nextval('_news_seq'::regclass) NOT NULL,
    datetime timestamp without time zone,
    title character varying(255),
    body text
);


ALTER TABLE public._news OWNER TO usernews;

--
-- Name: _rss; Type: TABLE; Schema: public; Owner: usernews; Tablespace: 
--

CREATE TABLE _rss (
    datetime timestamp without time zone,
    link character varying(255) NOT NULL,
    title character varying(255) NOT NULL,
    description text
);


ALTER TABLE public._rss OWNER TO usernews;

--
-- Name: _user; Type: TABLE; Schema: public; Owner: usernews; Tablespace: 
--

CREATE TABLE _user (
    name character varying(50) NOT NULL,
    "login" character varying(15) NOT NULL,
    passwd character varying(15) NOT NULL
);


ALTER TABLE public._user OWNER TO usernews;

--
-- Data for Name: _news; Type: TABLE DATA; Schema: public; Owner: usernews
--

COPY _news (code, datetime, title, body) FROM stdin;
1	2008-04-18 17:00:00	titulo 01	corpo 01
2	2008-04-18 17:00:00	titulo 02	corpo 02
4	2008-04-18 19:18:00	aaaaaaaaaaaaa	ccccccccccccccccccc
6	2008-04-18 22:10:00	Titulo da noticia	jkdskjdsjkdskjdskjdsdskjds\r\ndslkdsldslkds\r\nksdlksdlksdsd
\.


--
-- Data for Name: _rss; Type: TABLE DATA; Schema: public; Owner: usernews
--

COPY _rss (datetime, link, title, description) FROM stdin;
2008-04-18 20:34:47.834537	http://	titulo de rss	descricao rss
\.


--
-- Data for Name: _user; Type: TABLE DATA; Schema: public; Owner: usernews
--

COPY _user (name, "login", passwd) FROM stdin;
\.


--
-- Name: _news_pkey; Type: CONSTRAINT; Schema: public; Owner: usernews; Tablespace: 
--

ALTER TABLE ONLY _news
    ADD CONSTRAINT _news_pkey PRIMARY KEY (code);


--
-- Name: _rss_pkey; Type: CONSTRAINT; Schema: public; Owner: usernews; Tablespace: 
--

ALTER TABLE ONLY _rss
    ADD CONSTRAINT _rss_pkey PRIMARY KEY (link);


--
-- Name: _user_pkey; Type: CONSTRAINT; Schema: public; Owner: usernews; Tablespace: 
--

ALTER TABLE ONLY _user
    ADD CONSTRAINT _user_pkey PRIMARY KEY ("login");


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

