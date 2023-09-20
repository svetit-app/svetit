import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { TempSpaceComponent } from './component';

describe('TempSpaceComponent', () => {
  let component: TempSpaceComponent;
  let fixture: ComponentFixture<TempSpaceComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ TempSpaceComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TempSpaceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
