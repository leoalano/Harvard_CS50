SELECT movies.title, ratings.rating FROM movies JOIN ratings WHERE movies.year=2010 AND movies.id=ratings.movie_id AND ratings.rating>0 ORDER BY rating DESC, title;