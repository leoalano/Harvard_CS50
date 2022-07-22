SELECT DISTINCT title FROM movies JOIN ratings JOIN people JOIN stars WHERE movies.id=ratings.movie_id AND people.name='Chadwick Boseman' AND people.id=stars.person_id AND stars.movie_id=movies.id ORDER BY ratings.rating DESC LIMIT 5;