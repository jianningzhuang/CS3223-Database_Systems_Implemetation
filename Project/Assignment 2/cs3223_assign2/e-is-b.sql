BEGIN;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

SET enable_bitmapscan = OFF;
SET enable_indexscan = ON;
SET enable_indexonlyscan = ON;
SET enable_seqscan = OFF;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

DROP index cb_idx;

EXPLAIN ANALYZE  SELECT * FROM r WHERE b=9 AND c>=0;

ROLLBACK;