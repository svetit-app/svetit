import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { TempAddSpaceComponent } from './component';

describe('TempAddSpaceComponent', () => {
  let component: TempAddSpaceComponent;
  let fixture: ComponentFixture<TempAddSpaceComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ TempAddSpaceComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TempAddSpaceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
