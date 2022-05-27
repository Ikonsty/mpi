from random import uniform

class Grid:
    def __init__(self, n_col: int, n_row: int, t_range: tuple) -> None:
        self.n_col = n_col
        self.n_row = n_row
        self.min_t = t_range[0]
        self.max_t = t_range[1]
        self.grid = [ [0 for r in range(n_col)] for c in range(n_row) ]

        self.grid_constructor(self.n_col, self.n_row, self.min_t, self.max_t)

    def grid_constructor(self, n_col: int, n_row: int, min_t: float, max_t: float) -> None:
        # Create grid of initial conditions
        for r in range(n_row):
            for c in range(n_col):
                self.grid[r][c] = round(uniform(min_t, max_t), 10)

    def __str__(self) -> str:
        st = ''
        st += str(self.n_col) + ' ' + str(self.n_row) + '\n'
        for row in self.grid:
            for el in row:
                st += str(el) + ' '
        return st[:-1]
    
    def save_grid(self, filename) -> None:
        f = open(filename, "w")
        f.write(str(self))
        f.close()

if __name__ == '__main__':
    g = Grid(6, 4, (-10, 20))
    g.save_grid("grid_test.txt")