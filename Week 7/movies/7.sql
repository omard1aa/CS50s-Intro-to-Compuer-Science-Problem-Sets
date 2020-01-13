select title, rating from movies join ratings on movies.id = ratings.movie_id where year = 2010 order by ratings.rating desc, title asc;

