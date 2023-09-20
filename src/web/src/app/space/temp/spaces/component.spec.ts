import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { TempSpacesComponent } from './component';

describe('TempSpacesComponent', () => {
  let component: TempSpacesComponent;
  let fixture: ComponentFixture<TempSpacesComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ TempSpacesComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TempSpacesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
