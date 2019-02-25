type pageInfo = {
  startCursor: option(string),
  hasPreviousPage: bool,
  hasNextPage: bool,
  endCursor: option(string),
  total: option(int),
};

type edge('a) = {
  cursor: string,
  node: 'a,
};

type t('a) = {
  pageInfo,
  edges: option(list(edge('a))),
};
