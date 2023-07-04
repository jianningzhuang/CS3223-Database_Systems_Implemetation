BEGIN;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

SET enable_bitmapscan = ON;
SET enable_indexscan = ON;
SET enable_indexonlyscan = ON;
SET enable_seqscan = ON;

SHOW enable_bitmapscan;
SHOW enable_indexscan;
SHOW enable_indexonlyscan;
SHOW enable_seqscan;

DROP index c_idx;

EXPLAIN ANALYZE  SELECT * FROM r WHERE c=10;

ROLLBACK;