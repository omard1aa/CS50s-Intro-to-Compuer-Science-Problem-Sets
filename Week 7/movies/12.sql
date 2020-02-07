select title from movies join stars on movies.id=stars.movie_id 
join people on stars.person_id = people.id where people.name = 'Helena Bonham Carter' and movies.title in 
(select title from people join stars on people.id = stars.person_id join movies on stars.movie_id = movies.id 
 where people.name = 'Johnny Depp');
