BEGIN;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

SET enable_bitmapscan = ON;
SET enable_indexscan = ON;
SET enable_indexonlyscan = OFF;
SET enable_seqscan = ON;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

EXPLAIN ANALYZE  SELECT b FROM r WHERE c>15;

ROLLBACK;