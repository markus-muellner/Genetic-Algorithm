FROM gcc:7.4 as algorithm
RUN mkdir /algorithm
ADD src /src
WORKDIR /src
RUN make tests
RUN make
RUN make so
RUN mv tests /algorithm/tests
RUN mv main /algorithm/main
RUN mv ga.so /algorithm/ga.so
RUN mv in.csv /algorithm/in.csv
WORKDIR /algorithm
RUN touch out.csv
CMD ./tests; ./main

