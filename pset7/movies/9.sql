SELECT DISTINCT name FROM people JOIN movies JOIN stars WHERE movies.year=2004 AND people.id=person_id AND movies.id=stars.movie_id ORDER BY birth;