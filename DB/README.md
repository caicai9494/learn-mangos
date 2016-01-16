#MySQL

1. Create DB from file
  ```
  mysql -u root -p < start_up_script.sql
  ```

2. MySQL Keywords
 * SELECT DISTINCT column1, ...
   * DISTINCT treats two NULL-s as equal, thus duplicate
   * DISTINCT can be followed by multiply columns
 * NULL
   * Cannot compare using =(equal) sign
   * WHERE nullColumn IS NULL
   * WHERE nullColumn IS NOT NULL
 * GROUP BY column1
   * This is very similar to DISTINCT column. The major difference is that GROUP BY will sort the entries
 * SUM, COUNT, AVG, ...
   * Aggregate functions. Work the best with DISTINCT and GROUP BY.
 * LIMIT n
   * Select first n entries
   * n must be non-negative integer
 * FROM, WHERE
 * ORDER BY columns [ASC|DESC], ...
 * AS 
   * create an alias
 * FORMAT(arithmetic, precision)
   * turn into string
 * ROUND
 

3. SELECT 
```
SELECT column1, column2, ...
FROM
WHERE 
ORDER BY column1 [ASC|DESC]
```
