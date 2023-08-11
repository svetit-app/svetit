import { TestBed } from '@angular/core/testing';

import { FavService } from './fav.service';

describe('FavService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: FavService = TestBed.get(FavService);
    expect(service).toBeTruthy();
  });
});
