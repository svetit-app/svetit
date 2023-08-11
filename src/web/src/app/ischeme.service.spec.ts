import { TestBed, inject } from '@angular/core/testing';

import { SchemeService } from './scheme.service';

describe('SchemeService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [SchemeService]
    });
  });

  it('should be created', inject([SchemeService], (service: SchemeService) => {
    expect(service).toBeTruthy();
  }));
});
